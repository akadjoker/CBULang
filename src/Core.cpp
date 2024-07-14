#include "pch.h"
#include "Core.hpp"
#include "Utils.hpp"
#include <rlgl.h>
#include "Interpreter.hpp"
#include "Math.hpp"

void RenderQuad(const rQuad *quad)
{

    rlCheckRenderBatchLimit(4); // Make sure there is enough free space on the batch buffer
    rlSetTexture(quad->tex.id);

    rlBegin(RL_QUADS);

    Color a = quad->v[1].col;
    Color b = quad->v[0].col;
    Color c = quad->v[3].col;
    Color d = quad->v[2].col;

    rlNormal3f(0.0f, 0.0f, 1.0f);

    // rlColor4ub(255,255,255,255);
    rlColor4ub(a.r, a.g, a.b, a.a);
    rlTexCoord2f(quad->v[1].tx, quad->v[1].ty);
    rlVertex3f(quad->v[1].x, quad->v[1].y, quad->v[1].z);

    rlColor4ub(b.r, b.g, b.b, b.a);
    rlTexCoord2f(quad->v[0].tx, quad->v[0].ty);
    rlVertex3f(quad->v[0].x, quad->v[0].y, quad->v[0].z);

    rlColor4ub(c.r, c.g, c.b, c.a);
    rlTexCoord2f(quad->v[3].tx, quad->v[3].ty);
    rlVertex3f(quad->v[3].x, quad->v[3].y, quad->v[3].z);

    rlColor4ub(d.r, d.g, d.b, d.a);
    rlTexCoord2f(quad->v[2].tx, quad->v[2].ty);
    rlVertex3f(quad->v[2].x, quad->v[2].y, quad->v[2].z);

    rlEnd();
}

void RenderTransform(Texture2D texture, const Matrix2D *matrix, int blend)
{

    rQuad quad;
    quad.tex = texture;
    quad.blend = blend;

    float u = 0.0f;
    float v = 0.0f;
    float u2 = 1.0f;
    float v2 = 1.0f;

    float TempX1 = 0;
    float TempY1 = 0;
    float TempX2 = texture.width;
    float TempY2 = texture.height;

    quad.v[1].x = TempX1;
    quad.v[1].y = TempY1;
    quad.v[1].tx = u;
    quad.v[1].ty = v;

    quad.v[0].x = TempX1;
    quad.v[0].y = TempY2;
    quad.v[0].tx = u;
    quad.v[0].ty = v2;

    quad.v[3].x = TempX2;
    quad.v[3].y = TempY2;
    quad.v[3].tx = u2;
    quad.v[3].ty = v2;

    quad.v[2].x = TempX2;
    quad.v[2].y = TempY1;
    quad.v[2].tx = u2;
    quad.v[2].ty = v;

    for (int i = 0; i < 4; i++)
    {
        float x = quad.v[i].x;
        float y = quad.v[i].y;
        quad.v[i].x = matrix->a * x + matrix->c * y + matrix->tx;
        quad.v[i].y = matrix->d * y + matrix->b * x + matrix->ty;
    }

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0.0f;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = WHITE;

    RenderQuad(&quad);
}

void RenderTransformFlip(Texture2D texture, Rectangle clip, float z, bool flipX, bool flipY, Color color, const Matrix2D *matrix, int blend)
{

    rQuad quad;
    quad.tex = texture;
    quad.blend = blend;

    int widthTex = texture.width;
    int heightTex = texture.height;

    float left;
    float right;
    float top;
    float bottom;

    if (FIX_ARTIFACTS_BY_STRECHING_TEXEL)
    {
        left = (2 * clip.x + 1) / (2 * widthTex);
        right = left + (clip.width * 2 - 2) / (2 * widthTex);
        top = (2 * clip.y + 1) / (2 * heightTex);
        bottom = top + (clip.height * 2 - 2) / (2 * heightTex);
    }
    else
    {
        left = clip.x / widthTex;
        right = (clip.x + clip.width) / widthTex;
        top = clip.y / heightTex;
        bottom = (clip.y + clip.height) / heightTex;
    }

    if (flipX)
    {
        float tmp = left;
        left = right;
        right = tmp;
    }

    if (flipY)
    {
        float tmp = top;
        top = bottom;
        bottom = tmp;
    }

    float TempX1 = 0;
    float TempY1 = 0;
    float TempX2 = texture.width;
    float TempY2 = texture.height;

    quad.v[1].x = TempX1;
    quad.v[1].y = TempY1;
    quad.v[1].tx = left;
    quad.v[1].ty = top;

    quad.v[0].x = TempX1;
    quad.v[0].y = TempY2;
    quad.v[0].tx = left;
    quad.v[0].ty = bottom;

    quad.v[3].x = TempX2;
    quad.v[3].y = TempY2;
    quad.v[3].tx = right;
    quad.v[3].ty = bottom;

    quad.v[2].x = TempX2;
    quad.v[2].y = TempY1;
    quad.v[2].tx = right;
    quad.v[2].ty = top;

    for (int i = 0; i < 4; i++)
    {
        float x = quad.v[i].x;
        float y = quad.v[i].y;
        quad.v[i].x = matrix->a * x + matrix->c * y + matrix->tx;
        quad.v[i].y = matrix->d * y + matrix->b * x + matrix->ty;
    }

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0.0f;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = color;

    RenderQuad(&quad);
}

void RenderTransformFlipClip(Texture2D texture, int width, int height, Rectangle clip, bool flipX, bool flipY, Color color, const Matrix2D *matrix, int blend)
{

    rQuad quad;
    quad.tex = texture;
    quad.blend = blend;

    int widthTex = texture.width;
    int heightTex = texture.height;

    float left;
    float right;
    float top;
    float bottom;

    if (FIX_ARTIFACTS_BY_STRECHING_TEXEL)
    {
        left = (2 * clip.x + 1) / (2 * widthTex);
        right = left + (clip.width * 2 - 2) / (2 * widthTex);
        top = (2 * clip.y + 1) / (2 * heightTex);
        bottom = top + (clip.height * 2 - 2) / (2 * heightTex);
    }
    else
    {
        left = clip.x / widthTex;
        right = (clip.x + clip.width) / widthTex;
        top = clip.y / heightTex;
        bottom = (clip.y + clip.height) / heightTex;
    }

    if (flipX)
    {
        float tmp = left;
        left = right;
        right = tmp;
    }

    if (flipY)
    {
        float tmp = top;
        top = bottom;
        bottom = tmp;
    }

    float TempX1 = 0;
    float TempY1 = 0;
    float TempX2 = width;
    float TempY2 = height;

    quad.v[1].x = TempX1;
    quad.v[1].y = TempY1;
    quad.v[1].tx = left;
    quad.v[1].ty = top;

    quad.v[0].x = TempX1;
    quad.v[0].y = TempY2;
    quad.v[0].tx = left;
    quad.v[0].ty = bottom;

    quad.v[3].x = TempX2;
    quad.v[3].y = TempY2;
    quad.v[3].tx = right;
    quad.v[3].ty = bottom;

    quad.v[2].x = TempX2;
    quad.v[2].y = TempY1;
    quad.v[2].tx = right;
    quad.v[2].ty = top;

    for (int i = 0; i < 4; i++)
    {
        float x = quad.v[i].x;
        float y = quad.v[i].y;
        quad.v[i].x = matrix->a * x + matrix->c * y + matrix->tx;
        quad.v[i].y = matrix->d * y + matrix->b * x + matrix->ty;
    }

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0.0f;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = color;

    RenderQuad(&quad);
}
void RenderNormal(Texture2D texture, float x, float y, int blend)
{

    rQuad quad;
    quad.tex = texture;
    quad.blend = blend;

    float u = 0.0f;
    float v = 0.0f;
    float u2 = 1.0f;
    float v2 = 1.0f;

    float fx2 = x + texture.width;
    float fy2 = y + texture.height;

    quad.v[1].x = x;
    quad.v[1].y = y;
    quad.v[1].tx = u;
    quad.v[1].ty = v;

    quad.v[0].x = x;
    quad.v[0].y = fy2;
    quad.v[0].tx = u;
    quad.v[0].ty = v2;

    quad.v[3].x = fx2;
    quad.v[3].y = fy2;
    quad.v[3].tx = u2;
    quad.v[3].ty = v2;

    quad.v[2].x = fx2;
    quad.v[2].y = y;
    quad.v[2].tx = u2;
    quad.v[2].ty = v;

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0.0f;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = WHITE;

    RenderQuad(&quad);
}
void RenderTile(Texture2D texture, float x, float y, float width, float height, Rectangle clip, bool flipx, bool flipy, int blend)
{

    float fx2 = x + width;
    float fy2 = y + height;
    rQuad quad;
    quad.tex = texture;
    quad.blend = blend;

    int widthTex = texture.width;
    int heightTex = texture.height;

    float left, right, top, bottom;

    if (FIX_ARTIFACTS_BY_STRECHING_TEXEL)
    {

        left = (2 * clip.x + 1) / (2 * widthTex);
        right = left + (clip.width * 2 - 2) / (2 * widthTex);
        top = (2 * clip.y + 1) / (2 * heightTex);
        bottom = top + (clip.height * 2 - 2) / (2 * heightTex);
    }
    else
    {
        left = clip.x / widthTex;
        right = (clip.x + clip.width) / widthTex;
        top = clip.y / heightTex;
        bottom = (clip.y + clip.height) / heightTex;
    }

    if (flipx)
    {
        float tmp = left;
        left = right;
        right = tmp;
    }

    if (flipy)
    {
        float tmp = top;
        top = bottom;
        bottom = tmp;
    }

    quad.v[1].tx = left;
    quad.v[1].ty = top;
    quad.v[1].x = x;
    quad.v[1].y = y;

    quad.v[0].x = x;
    quad.v[0].y = fy2;
    quad.v[0].tx = left;
    quad.v[0].ty = bottom;

    quad.v[3].x = fx2;
    quad.v[3].y = fy2;
    quad.v[3].tx = right;
    quad.v[3].ty = bottom;

    quad.v[2].x = fx2;
    quad.v[2].y = y;
    quad.v[2].tx = right;
    quad.v[2].ty = top;

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0.0f;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = WHITE;

    RenderQuad(&quad);
}

bool find_word(const std::string &haystack, const std::string &needle)
{
    size_t index = haystack.find(needle);
    if (index == std::string::npos)
        return false;

    auto not_part_of_word = [&](size_t idx)
    {
        if (idx >= haystack.size())
            return true;
        if (std::isspace(haystack[idx]) || std::ispunct(haystack[idx]))
            return true;
        return false;
    };

    return not_part_of_word(index - 1) && not_part_of_word(index + needle.size());
}

//************************************************************/
//********************** VEC2" ******************************/
//************************************************************/
Vec2 &Vec2::add(const Vec2 &other)
{
    x += other.x;
    y += other.y;

    return *this;
}

Vec2 &Vec2::subtract(const Vec2 &other)
{
    x -= other.x;
    y -= other.y;

    return *this;
}

Vec2 &Vec2::multiply(const Vec2 &other)
{
    x *= other.x;
    y *= other.y;

    return *this;
}

Vec2 &Vec2::divide(const Vec2 &other)
{
    x /= other.x;
    y /= other.y;

    return *this;
}

Vec2 &Vec2::add(float value)
{
    x += value;
    y += value;

    return *this;
}

Vec2 &Vec2::subtract(float value)
{
    x -= value;
    y -= value;

    return *this;
}

Vec2 &Vec2::multiply(float value)
{
    x *= value;
    y *= value;

    return *this;
}

Vec2 &Vec2::divide(float value)
{
    x /= value;
    y /= value;

    return *this;
}

Vec2 operator+(Vec2 left, const Vec2 &right)
{
    return left.add(right);
}

Vec2 operator-(Vec2 left, const Vec2 &right)
{
    return left.subtract(right);
}

Vec2 operator*(Vec2 left, const Vec2 &right)
{
    return left.multiply(right);
}

Vec2 operator/(Vec2 left, const Vec2 &right)
{
    return left.divide(right);
}

Vec2 operator+(Vec2 left, float value)
{
    return Vec2(left.x + value, left.y + value);
}

Vec2 operator-(Vec2 left, float value)
{
    return Vec2(left.x - value, left.y - value);
}

Vec2 operator*(Vec2 left, float value)
{
    return Vec2(left.x * value, left.y * value);
}

Vec2 operator/(Vec2 left, float value)
{
    return Vec2(left.x / value, left.y / value);
}

Vec2 &Vec2::operator+=(const Vec2 &other)
{
    return add(other);
}

Vec2 &Vec2::operator-=(const Vec2 &other)
{
    return subtract(other);
}

Vec2 &Vec2::operator*=(const Vec2 &other)
{
    return multiply(other);
}

Vec2 &Vec2::operator/=(const Vec2 &other)
{
    return divide(other);
}

Vec2 &Vec2::operator+=(float value)
{
    return add(value);
}

Vec2 &Vec2::operator-=(float value)
{
    return subtract(value);
}

Vec2 &Vec2::operator*=(float value)
{
    return multiply(value);
}

Vec2 &Vec2::operator/=(float value)
{
    return divide(value);
}

bool Vec2::operator==(const Vec2 &other) const
{
    return x == other.x && y == other.y;
}

bool Vec2::operator!=(const Vec2 &other) const
{
    return !(*this == other);
}

bool Vec2::operator<(const Vec2 &other) const
{
    return x < other.x && y < other.y;
}

bool Vec2::operator<=(const Vec2 &other) const
{
    return x <= other.x && y <= other.y;
}

bool Vec2::operator>(const Vec2 &other) const
{
    return x > other.x && y > other.y;
}

bool Vec2::operator>=(const Vec2 &other) const
{
    return x >= other.x && y >= other.y;
}

float Vec2::distance(const Vec2 &other) const
{
    float a = x - other.x;
    float b = y - other.y;
    return sqrt(a * a + b * b);
}

float Vec2::dot(const Vec2 &other) const
{
    return x * other.x + y * other.y;
}

float Vec2::magnitude() const
{
    return sqrt(x * x + y * y);
}

Vec2 Vec2::normalised() const
{
    float length = magnitude();
    return Vec2(x / length, y / length);
}

//************************************************************/
//********************** MATRIX2D ******************************/
//************************************************************/

Matrix2D::Matrix2D()
{
    a = 1;
    b = 0;
    c = 0;
    d = 1;
    tx = 0;
    ty = 0;
}

Matrix2D::~Matrix2D()
{
}

void Matrix2D::Identity()
{
    a = 1;
    b = 0;
    c = 0;
    d = 1;
    tx = 0;
    ty = 0;
}

void Matrix2D::Set(float a, float b, float c, float d, float tx, float ty)
{

    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
    this->tx = tx;
    this->ty = ty;
}

void Matrix2D::Concat(const Matrix2D &m)
{
    float a1 = this->a * m.a + this->b * m.c;
    this->b = this->a * m.b + this->b * m.d;
    this->a = a1;

    float c1 = this->c * m.a + this->d * m.c;
    this->d = this->c * m.b + this->d * m.d;

    this->c = c1;

    float tx1 = this->tx * m.a + this->ty * m.c + m.tx;
    this->ty = this->tx * m.b + this->ty * m.d + m.ty;
    this->tx = tx1;
}

Vec2 Matrix2D::TransformCoords(Vec2 point)
{

    Vec2 v;

    v.x = this->a * point.x + this->c * point.y + this->tx;
    v.y = this->d * point.y + this->b * point.x + this->ty;

    return v;
}

Matrix2D Matrix2D::Mult(const Matrix2D &m)
{
    Matrix2D result;

    result.a = this->a * m.a + this->b * m.c;
    result.b = this->a * m.b + this->b * m.d;
    result.c = this->c * m.a + this->d * m.c;
    result.d = this->c * m.b + this->d * m.d;

    result.tx = this->tx * m.a + this->ty * m.c + this->tx;
    result.ty = this->tx * m.b + this->ty * m.d + this->ty;

    return result;
}

void Matrix2D::Rotate(float angle)
{
    float acos = cos(angle);
    float asin = sin(angle);

    float a1 = this->a * acos - this->b * asin;
    this->b = this->a * asin + this->b * acos;
    this->a = a1;

    float c1 = this->c * acos - this->d * asin;
    this->d = this->c * asin + this->d * acos;
    this->c = c1;

    float tx1 = this->tx * acos - this->ty * asin;
    this->ty = this->tx * asin + this->ty * acos;
    this->tx = tx1;
}

void Matrix2D::Scale(float x, float y)
{
    this->a *= x;
    this->b *= y;

    this->c *= x;
    this->d *= y;

    this->tx *= x;
    this->ty *= y;
}

void Matrix2D::Translate(float x, float y)
{
    this->tx += x;
    this->ty += y;
}

void Matrix2D::Skew(float skewX, float skewY)
{
    float sinX = sin(skewX);
    float cosX = cos(skewX);
    float sinY = sin(skewY);
    float cosY = cos(skewY);

    Set(
        this->a * cosY - this->b * sinX,
        this->a * sinY + this->b * cosX,
        this->c * cosY - this->d * sinX,
        this->c * sinY + this->d * cosX,
        this->tx * cosY - this->ty * sinX,
        this->tx * sinY + this->ty * cosX);
}

//************************************************************/
//********************** MATRIX2D ****************************/
//************************************************************/

Instance::~Instance()
{
    if (mask)
        delete mask;
}

Instance::Instance()
{

    color = WHITE;
    scale = Vec2(1.0f, 1.0f);
    skew = Vec2(0.0f);
    width = 1;
    height = 1;
    originX = 0;
    originY = 0;
    name = "";
    x = 0;
    y = 0;
    visible = true;
    collidable = false;
    pivot = Vec2(0, 0);
    mask = nullptr;
    matrix = GetWorldTransformation();
}

void Instance::Update(double delta)
{

    // for (auto e : m_colide_entities)
    //      OnCollide(e);

    // m_colide_entities.clear();
}
void Instance::Render()
{

    if (solid)
    {
        int bX = (int)x - originX;
        int bY = (int)y - originY;
        DrawRectangleLines(bX, bY, width, height, RED);
        return;
    }

    matrix = GetWorldTransformation();

    if (graph >= 0)
    {
        Graph gr = Scene::Get().getGraph(graph);
        if (gr.id == graph)
        {
            size.x = gr.clip.width;
            size.y = gr.clip.height;
            RenderTransformFlip(gr.texture, gr.clip, 0.0f, flip_x, flip_y, color, &matrix, 1);
            // DrawTexture(gr.texture,m_x,m_y,m_color);
        }
    }
    else
    {
        // Vec2 p =GetPoint(m_pivot.x,m_pivot.y);
        // DrawCircleLines((int)p.x,(int)p.y,10,RED);
    }

    // bool showBox= showPivot = true;

    if (showPivot)
    {
        Vec2 p = GetPoint(pivot.x, pivot.y);
        DrawCircleLines((int)p.x, (int)p.y, 6, RED);
    }

    if (showBox)
    {
        int bX = getWorldX() - getWorldOriginX();
        int bY = getWorldY() - getWorldOriginY();
        DrawRectangleLines(bX, bY, width, height, RED);
    }
}

// void Instance::addChild(Instance *c)
// {
//     if (c != nullptr)
//         m_childs.push_back(c);
// }

// bool Instance::RemoveChild(Instance *obj)
// {
//     auto e = std::remove_if(m_childs.begin(), m_childs.end(), [&](Instance *a)
//                             { return *a == *obj; });
//     if (e != m_childs.end())
//     {
//         obj->m_parent = nullptr;
//         m_childs.erase(e);
//         return true;
//     }
//     return false;
// }

void Instance::Destroy() // add on the list to remove
{
    // for (size_t i = 0; i < m_childs.size(); i++)
    // {
    //     Instance *e = m_childs[i];
    //     e->Destroy();
    //     m_childs.erase(m_childs.begin() + i);
    // }
    alive = false;
    m_colide_entities.clear();
    // m_childs.clear();
}

Vec2 Instance::get_point(size_t index)
{
    Vec2 v;
    if (graph > 0)
    {
        Graph gr = Scene::Get().getGraph(graph);
        if (gr.id == graph)
        {
            if (index > gr.points.size())
                return v;
            v = gr.points[index];
        }
    }
    return v;
}

Vec2 Instance::get_real_point(size_t index)
{
    Vec2 v;
    Graph gr = Scene::Get().getGraph(graph);
    if (gr.id == graph)
    {
        if (index > gr.points.size())
            return v;
        Vec2 p = gr.points[index];
        v = GetWorldPoint(p.x, p.y);
    }

    return v;
}

Vec2 Instance::GetPoint(double _x, double _y)
{
    return matrix.TransformCoords(Vec2(_x, _y));
}

void Instance::centerOrigin()
{
    originX = width / 2;
    originY = height / 2;
    this->pivot.x = this->size.x / 2;
    this->pivot.y = this->size.y / 2;
}
void Instance::setOrigin(int _x, int _y)
{
    originX = _x;
    originY = _y;
}

void Instance::setPivot(double x, double y)
{
    this->pivot.x = x;
    this->pivot.y = y;
}

void Instance::CenterPivot()
{
    this->pivot.x = this->size.x / 2;
    this->pivot.y = this->size.y / 2;
    originX = width / 2;
    originY = height / 2;
}

void Instance::setHitbox(int width, int height, int originX, int originY)
{
    this->width = width;
    this->height = height;
    this->originX = originX;
    this->originY = originY;
    this->collidable = true;
}
void Instance::setGraph(int id)
{
    graph = id;
    Graph gr = Scene::Get().getGraph(id);
    size.x = gr.clip.width;
    size.y = gr.clip.height;
    setHitbox((int)gr.clip.width, (int)gr.clip.height);
    centerOrigin();
    CenterPivot();
}

Matrix2D Instance::GetLocalTrasformation()
{
    local_transform.Identity();

    if (skew.x == 0.0f && skew.y == 0.0f)
    {

        if (angle == 0.0)
        {

            local_transform.Set(scale.x, 0.0, 0.0, scale.y, x - pivot.x * scale.x, y - pivot.y * scale.y);
        }
        else
        {
            float acos = cos(angle * RAD);
            float asin = sin(angle * RAD);
            float a = scale.x * acos;
            float b = scale.x * asin;
            float c = scale.y * -asin;
            float d = scale.y * acos;
            float tx = x - pivot.x * a - pivot.y * c;
            float ty = y - pivot.x * b - pivot.y * d;

            local_transform.Set(a, b, c, d, tx, ty);
        }
    }
    else
    {

        local_transform.Identity();
        local_transform.Scale(scale.x, scale.y);
        local_transform.Skew(skew.x, skew.y);
        local_transform.Rotate(angle * RAD);
        local_transform.Translate(x, y);

        if (pivot.x != 0.0f || pivot.y != 0.0f)
        {

            local_transform.tx = x - local_transform.a * pivot.x - local_transform.c * pivot.y;
            local_transform.ty = y - local_transform.b * pivot.x - local_transform.d * pivot.y;
        }
    }

    return local_transform;
}

Matrix2D Matrix2DMult(const Matrix2D curr, const Matrix2D m)
{

    Matrix2D result;

    result.a = curr.a * m.a + curr.b * m.c;
    result.b = curr.a * m.b + curr.b * m.d;
    result.c = curr.c * m.a + curr.d * m.c;
    result.d = curr.c * m.b + curr.d * m.d;

    result.tx = curr.tx * m.a + curr.ty * m.c + m.tx;
    result.ty = curr.tx * m.b + curr.ty * m.d + m.ty;

    return result;
}

Matrix2D Instance::GetWorldTransformation()
{
    local_transform = GetLocalTrasformation();
    if (m_parent != nullptr)
    {
        Matrix2D mat = m_parent->GetWorldTransformation();
        wordl_transform = Matrix2DMult(local_transform, mat);
        return wordl_transform;
    }
    return local_transform;
}

Vec2 Instance::GetWorldPoint(double _x, double _y)
{
    return matrix.TransformCoords(Vec2(_x, _y));
}

Vec2 Instance::GetWorldPoint(Vec2 p)
{
    return matrix.TransformCoords(p);
}

Vec2 Instance::GetLocalPoint(Vec2 p)
{
    return GetLocalTrasformation().TransformCoords(p);
}

Vec2 Instance::GetLocalPoint(double x, double y)
{
    return GetLocalTrasformation().TransformCoords(Vec2(x, y));
}

void Instance::setParent(Instance *Parent)
{
    if (Parent != nullptr)
    {
        m_parent = Parent;
        // Parent->addChild(this);
    }
}

Instance *Instance::collide(double x, double y)
{
    if (!collidable)
        return nullptr;
    std::vector<Instance *> types = Scene::Get().GetEntities();
    for (size_t j = 0; j < types.size(); j++)
    {
        Instance *e = types[j];
        if (!e->collidable || e == this)
            continue;
        if (collideWith(e, x, y))
            return e;
    }
    return nullptr;
}

bool Instance::place_free(double x, double y)
{
    if (!collidable)
        return false;
    std::vector<Instance *> types = Scene::Get().GetEntities();
    for (size_t j = 0; j < types.size(); j++)
    {
        Instance *e = types[j];
        if (!e->collidable || e == this)
            continue;
        if (collideWith(e, x, y))
            return true;
    }
    return false;
}

bool Instance::place_meeting(double x, double y, std::string &name)
{
    if (!collidable)
        return false;

    std::vector<Instance *> types = Scene::Get().GetEntities();
    for (size_t j = 0; j < types.size(); j++)
    {
        Instance *e = types[j];
        if (!find_word(e->name, name))
            continue;
        if (!e->collidable || e == this)
            continue;

        if (collideWith(e, x, y))
        {
           // m_colide_entities.push_back(e);
            return true;
        }
    }
    return false;
}

double Instance::getWorldX() const
{
    if (m_parent != nullptr)
        return (m_parent->getWorldX() + x);
    return x;
}

double Instance::getWorldY() const
{

    if (m_parent != nullptr)
        return (m_parent->getWorldY() + y);
    return y;
}

double Instance::GetWorldAngle()
{
    if (m_parent)
        return m_parent->angle - angle;
    return angle;
}

double Instance::getWorldOriginX() const
{
    if (m_parent != nullptr)
        return (m_parent->getWorldOriginX() + originX);
    return originX;
}

double Instance::getWorldOriginY() const
{
    if (m_parent != nullptr)
        return (m_parent->getWorldOriginX() + originY);
    return originY;
}

bool Instance::collideWithRect(const Rectangle e, double x, double y, int Pivot_x, int Pivot_y)

{
    _x = this->x;
    _y = this->y;
    this->x = x;
    this->y = y;

    if (x - this->getWorldOriginX() + width > e.x - Pivot_x && y - this->getWorldOriginY() + height > e.y - Pivot_y && x - this->getWorldOriginX() < e.x - Pivot_x + e.width && y - this->getWorldOriginY() < e.y - Pivot_y + e.height && collidable && alive)
    {

        this->x = _x;
        this->y = _y;

        return true;
    }
    this->x = _x;
    this->y = _y;
    return false;
}

void Instance::Kill()
{
    alive = false;
}

bool Instance::collideWith(const Instance *e, double x, double y)
{
    _x = this->x;
    _y = this->y;
    this->x = x;
    this->y = y;

    if ((x - this->getWorldOriginX() + width > e->getWorldX() - e->getWorldOriginX()) &&
        (y - this->getWorldOriginY() + height > e->getWorldY() - e->getWorldOriginY()) &&
        (x - this->getWorldOriginX() < e->getWorldX() - e->getWorldOriginX() + e->width) &&
        (y - this->getWorldOriginY() < e->getWorldY() - e->getWorldOriginY() + e->height) &&
        (collidable && e->collidable && e != this && alive && e->alive))
    {

        if (mask != nullptr && e->mask != nullptr)
        {
            if (!e->mask || e->mask->collide(mask))
            {
                this->x = _x;
                this->y = _y;

                return true;
            }
        }
        else
        {
            this->x = _x;
            this->y = _y;

            return true;
        }
    }
    this->x = _x;
    this->y = _y;
    return false;
}

//*******************************************************************************************************************

//*******************************************************************************************************************

bool Mask::collide(const Mask *other)
{
    return true;
}
bool Mask::collideMask(const Mask *other)
{

    return parent->getWorldX() - parent->originX + parent->width > other->parent->getWorldX() - other->parent->originX && parent->getWorldY() - parent->originY + parent->height > other->parent->getWorldY() - other->parent->originY && parent->getWorldX() - parent->originX < other->parent->getWorldX() - other->parent->originX + other->parent->width && parent->getWorldY() - parent->originY < other->parent->getWorldY() - other->parent->originY + other->parent->height;
}

bool Hitbox::collideHitbox(const Hitbox *other)
{

    return parent->getWorldX() - parent->originX + parent->width > other->parent->getWorldX() - other->parent->originX && parent->getWorldY() - parent->originY + parent->height > other->parent->getWorldY() - other->parent->originY && parent->getWorldX() - parent->originX < other->parent->getWorldX() - other->parent->originX + other->parent->width && parent->getWorldY() - parent->originY < other->parent->getWorldY() - other->parent->originY + other->parent->height;
}
bool Hitbox::collideMask(const Mask *other)
{
    return parent->getWorldX() - parent->originX + parent->width > other->parent->getWorldX() - other->parent->originX && parent->getWorldY() - parent->originY + parent->height > other->parent->getWorldY() - other->parent->originY && parent->getWorldX() - parent->originX < other->parent->getWorldX() - other->parent->originX + other->parent->width && parent->getWorldY() - parent->originY < other->parent->getWorldY() - other->parent->originY + other->parent->height;
}

//*******************************************************************************************************************
//*******************************************************************************************************************

template <typename T, typename Key>
bool key_exists(const T &container, const Key &key)
{
    return (container.find(key) != std::end(container));
}

Scene::Scene()
{

    Log(0, "*Create Scene");

    clearColor = BLACK;

    camera.target.x = static_cast<float>(GetScreenWidth() / 2.0f);
    camera.target.y = static_cast<float>(GetScreenHeight() / 2.0f);

    camera.offset.x = static_cast<float>(GetScreenWidth() / 2.0f);
    camera.offset.y = static_cast<float>(GetScreenHeight() / 2.0f);
    bound.x = 0;
    bound.y = 0;
    bound.width = GetScreenWidth();
    bound.height = GetScreenHeight();

    offset = Vec2(camera.offset.x, camera.offset.y);
    target = Vec2(camera.target.x, camera.target.y);
    rotation = 0.0f;
    zoom = 1.0f;

    camera.rotation = rotation;
    camera.zoom = zoom;
    camera.offset = (Vector2){offset.x, offset.y};
    camera.target = (Vector2){target.x, target.y};

    m_entities.reserve(100);
    addLayers(2);
}
void Scene::Clear()
{

    Log(0, "*Clear Scene");

    for (std::unordered_map<std::string, Texture2D>::const_iterator it = texture_list.begin(); it != texture_list.end(); it++)
    {
        std::string key = it->first;
        Texture2D value = it->second;
        Log(0, "Free Graph (%s)", key.c_str());
        UnloadTexture(value);
    }

    for (std::unordered_map<int, Graph>::const_iterator it = graphics.begin(); it != graphics.end(); it++)
    {
        int key = it->first;
        if (key >= 0)
        {
            Graph value = it->second;
            if (!value.isClip)
                UnloadImage(value.image);
            value.points.clear();
        }
    }

    for (int layer = 0; layer < layersCount(); layer++)
    {
        m_layers[layer].clear();
    }
    m_layers.clear();

    while (!m_entities.empty())
    {
        Instance *e = m_entities.back();
        m_entities.pop_back();
        e->Destroy();
        delete e;
    }

    m_entities.clear();
}

void Scene::message(int x, int y, Color c, const char *text, ...)
{
#define MAX_TEXTFORMAT_BUFFERS 4 // Maximum number of static buffers for text formatting
#define MAX_TEXT_BUFFER_LENGTH 1024

    // We create an array of buffers so strings don't expire until MAX_TEXTFORMAT_BUFFERS invocations
    static char buffers[MAX_TEXTFORMAT_BUFFERS][MAX_TEXT_BUFFER_LENGTH] = {0};
    static int index = 0;

    char *currentBuffer = buffers[index];
    memset(currentBuffer, 0, MAX_TEXT_BUFFER_LENGTH); // Clear buffer before using

    va_list args;
    va_start(args, text);
    vsnprintf(currentBuffer, MAX_TEXT_BUFFER_LENGTH, text, args);
    va_end(args);

    index += 1; // Move to next buffer for next function call
    if (index >= MAX_TEXTFORMAT_BUFFERS)
        index = 0;

    DrawText(currentBuffer, x, y, 12, c);
}
void Scene::Refresh()
{

    for (int layer = 0; layer < layersCount(); layer++)
    {

        m_layers[layer].erase(
            std::remove_if(std::begin(m_layers[layer]), std::end(m_layers[layer]),
                           [](Instance *entity)
                           {
                               return !entity->alive;
                           }),
            std::end(m_layers[layer]));
    }

    m_entities.erase(
        std::remove_if(std::begin(m_entities), std::end(m_entities),
                       [](Instance *entity)
                       {
                           if (entity->alive)
                               return false;

                           entity->Destroy();
                           delete entity;
                           entity = nullptr;
                           return true;
                       }),
        std::end(m_entities));
}

void Scene::CleanAndRemove()
{
    Clear();
}

void Scene::Update(float delta)
{
    bound.width = GetScreenWidth();
    bound.height = GetScreenHeight();

    for (auto e : m_entities)
    {
        if (e->alive)
            e->Update(delta);
    }
}

void Scene::UpdateAndRefresh()
{

    Refresh();

    ClearBackground(clearColor);

    camera.rotation = rotation;
    camera.zoom = zoom;
    camera.offset = (Vector2){offset.x, offset.y};
    camera.target = (Vector2){target.x, target.y};

    BeginMode2D(camera);

    Update(GetFrameTime());

    for (int i = 0; i < m_num_layers - 1; i++)
    {
        for (auto &e : m_layers[i])
        {
            if (e->alive)
                e->Render();
        }
    }

    EndMode2D();
    for (auto &e : m_layers[m_num_layers])
    {
        if (e->alive)
            e->Render();
    }

    DrawRectangle(10, GetScreenHeight() - 40, 250, 200, Fade(SKYBLUE, 0.5f));
    DrawRectangleLines(11, GetScreenHeight() - 40, 248, 200, BLUE);

    DrawFPS(GetScreenWidth() - 85, GetScreenHeight() - 22);

    DrawText(TextFormat("Scene Objects [%d] Layers [%d]", m_entities.size(), layersCount()), 25, GetScreenHeight() - 20, 10, BLACK);

    int Y = GetScreenHeight() - 40;

    int index = 0;
    for (int i = 0; i < layersCount(); i++)
    {

        if (m_layers[i].size() > 0)
        {
            Y = GetScreenHeight() - 40 - index * 20;
            DrawText(TextFormat("Layer [%d]  Objects [%d] ", i, m_layers[i].size()), 28, Y, 10, BLACK);
            index++;
        }
    }

    
}

Instance *Scene::CreateInstance(long ID, const std::string &name, int graph, double x, double y, double angle, int layer)

{
    Instance *e = new Instance();

    e->name = name;
    e->id = ID;
    e->x = x;
    e->y = y;
    e->alive = true;
    e->angle = angle;
    e->layer = layer;

    if (graph == -1)
        e->solid = true;
    else
        e->setGraph(graph);

    m_entities.emplace_back(e);
    addToLayer(e);
    return e;
}

Scene::~Scene()
{
}

int Scene::addLayer()
{
    std::vector<Instance *> l;
    l.reserve(100);
    m_layers.emplace(layersCount(), l);
    ++m_num_layers;
    return layersCount();
}

void Scene::addToLayer(Instance *e)
{
    if (e->layer > layersCount())
    {
        while (layersCount() <= e->layer)
        {
            addLayer();
        }
    }
    m_layers[e->layer].emplace_back(e);
}

int Scene::layersCount()
{
    return (int)m_layers.size();
}

int Scene::addLayers(int count)
{
    for (int i = 0; i < count; i++)
    {
        addLayer();
    }
    return layersCount();
}
//*//////////////////////////////////////////////////////
Instance *Scene::FindInstanceByName(const std::string &name)
{

    for (unsigned int n = 0; n < m_entities.size(); n++)
    {
        if (find_word(m_entities[n]->name, name))
            return (m_entities[n]);
    }
    Log(2, " Din't find (%s) process", name.c_str());
    return nullptr;
}

bool Scene::graphExists(int id)
{
    if (graphics.find(id) != graphics.end())
    {
        return false;
    }

    return false;
}
bool Scene::graph_add_point(int id, double x, double y)
{
    Graph gr = getGraph(id);
    if (gr.id == id)
    {
        gr.points.push_back(Vec2(x, y));
        return true;
    }
    return false;
}
int Scene::graph_total_points(int id)
{
    Graph gr = getGraph(id);
    if (gr.id == id)
        return (int)gr.points.size();
    return -1;
}
int Scene::loadGraphFromFile(const std::string &string)
{
    int index = 100;
    for (std::unordered_map<int, Graph>::const_iterator it = graphics.begin(); it != graphics.end(); it++)
    {
        int key = it->first;
        if (index == key)
            index++;
        else
            break;
    }
    addGraphFromFile(string, index);
    return index;
}

bool Scene::addGraphFromFile(const std::string &string, int id)
{
    if (id < 0)
        return false;

    if (FileExists(string.c_str()))
    {

        Graph gr;
        gr.name = std::string(GetFileNameWithoutExt(string.c_str()));

        if (key_exists(texture_list, gr.name))
        {
            gr.texture = texture_list.at(gr.name);
        }
        else
        {
            gr.texture = LoadTexture(string.c_str());
            Log(0, "Load image (%s)  id(%d)  data(%u) size (%d,%d) ", string.c_str(), id, gr.texture.id, gr.texture.width, gr.texture.height);
            texture_list.insert(std::pair<std::string, Texture>(gr.name, gr.texture));
        }

        gr.clip.x = 0;
        gr.clip.y = 0;
        gr.clip.width = gr.texture.width;
        gr.clip.height = gr.texture.height;
        gr.id = id;
        gr.points.push_back(Vec2(gr.texture.width / 2, gr.texture.height / 2));
        graphics.emplace(id, gr);
        return true;
    }

    else
    {

        Log(2, "File Image don't exits  File: %s", string.c_str());
        return false;
    }
    return false;
}

bool Scene::addSubGraphFromFile(int graph, int id, int x, int y, int w, int h)
{

    auto it = graphics.find(graph);
    if (it == graphics.end())
    {
        Log(2, "Graphic with ID %d don't exists", graph);
        return false;
    }

    Graph parent = getGraph(graph);
    Graph gr;
    gr.name = parent.name;
    gr.texture.format = parent.texture.format;
    gr.texture.height = parent.texture.height;
    gr.texture.width = parent.texture.width;
    gr.texture.format = parent.texture.id;
    gr.texture.mipmaps = parent.texture.mipmaps;
    gr.isClip = true;
    gr.clip.x = x;
    gr.clip.y = y;
    gr.clip.width = w;
    gr.clip.height = h;

    gr.points.push_back(Vec2(w / 2, h / 2));
    gr.id = id;
    Log(0, "Load Graphic (%s)  id(%d)  data(%u) size (%d,%d) ", gr.name.c_str(), id, gr.texture.id, gr.texture.width, gr.texture.height);
    graphics.emplace(id, gr);
    return true;
}

bool Scene::addAtlasGraphFromFile(const std::string &string, int id, int w, int h)
{
    auto it = graphics.find(id);
    if (it == graphics.end())
    {

        Graph gr;
        gr.name = std::string(GetFileNameWithoutExt(string.c_str()));
        if (key_exists(texture_list, gr.name))
            gr.texture = texture_list.at(gr.name);
        else
        {
            gr.image = LoadImage(string.c_str());
            gr.texture = LoadTextureFromImage(gr.image);
            Log(0, "Load (%s)  id(%d)  data(%u) size (%d,%d) ", string.c_str(), id, gr.texture.id, gr.image.width, gr.image.height);
            texture_list.insert(std::pair<std::string, Texture>(gr.name, gr.texture));
        }
        gr.clip.x = 0;
        gr.clip.y = 0;
        gr.clip.width = gr.texture.width;
        gr.clip.height = gr.texture.height;
        // = (Rectangle){0, 0, gr.texture.width, gr.texture.height};
        gr.id = id;

        graphics.emplace(id, gr);

        int idStart = id + 1;

        int cols = w; //(int)floor(gr.clip.width  / w);
        int rows = h; //(int)floor(gr.clip.height / h);
        float tile_w = (gr.clip.width / w);
        float tile_h = (gr.clip.width / h);

        Log(0, "Create atlas tile(%f:%f)", tile_w, tile_h);

        for (int y = 0; y < rows; y++)
        {

            for (int x = 0; x < cols; x++)
            {
                Graph clip;
                clip.name = std::string(TextFormat("%s_sub_%d", gr.name.c_str(), idStart));
                clip.id = idStart;
                clip.texture.format = gr.texture.format;
                clip.texture.height = gr.texture.height;
                clip.texture.width = gr.texture.width;
                clip.texture.id = gr.texture.id;
                clip.texture.mipmaps = gr.texture.mipmaps;
                clip.isClip = true;

                clip.clip.x = x * tile_w;
                clip.clip.y = y * tile_h;
                clip.clip.width = tile_w;
                clip.clip.height = tile_h;
                // = (Rectangle){x * tile_w, y * tile_h, tile_w, tile_h};
                graphics.emplace(clip.id, clip);

                Log(0, "Create sub image %s %d", clip.name.c_str(), clip.id);

                idStart++;
            }
        }
    }

    return true;
}

int Scene::getGraphWidth(int value)
{
    Graph gr = graphics[value];
    return gr.GetWidth();
}
int Scene::getGraphHeight(int value)
{
    Graph gr = graphics[value];
    return gr.GetHeight();
}

Graph Scene::getGraph(int id)
{
    return graphics[id];
}
void Scene::setCameraPosition(float x, float y)
{
    target.x = x;
    target.y = y;
}
void Scene::setCameraZoom(float value)
{
    zoom = value;
}

void Scene::setClearColor(int r, int g, int b)
{
    clearColor.r = r;
    clearColor.g = g;
    clearColor.b = b;
}

bool Scene::InScreen(Instance *e)
{
    float width = e->width * e->scale.x;
    float height = e->height * e->scale.y;
    float x = -e->getWorldOriginX() + e->getWorldX();
    float y = -e->getWorldOriginY() + e->getWorldY();

    float cameraLeft = camera.target.x - (bound.width / 2) - camera.offset.x;
    float cameraRight = camera.target.x + (bound.width / 2) + camera.offset.x;
    float cameraTop = camera.target.y - (bound.height / 2) - camera.offset.y;
    float cameraBottom = camera.target.y + (bound.height / 2) + camera.offset.y;

    // DrawRectangleLines((int)x, (int)y, (int)width, (int)height, RED);
    //  DrawRectangleLinesEx((Rectangle){cameraLeft, cameraTop, cameraRight, cameraBottom}, 1, GREEN);

    if (x > cameraLeft && x < cameraRight && y > cameraTop && y < cameraBottom)
    {
        if (x + width > cameraRight)
            return false;
        if (x < cameraLeft)
            return false;
        if (y + height > cameraBottom)
            return false;
        if (y < cameraTop)
            return false;
        return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

static LiteralPtr native_mouse_down(ExecutionContext *ctx, int argc)
{
    if (argc != 1)
    {
        return ctx->asBool(false);
    }
    int button = ctx->getInt(0);

    return ctx->asBool(IsMouseButtonDown(button));
}

static LiteralPtr native_mouse_up(ExecutionContext *ctx, int argc)
{
    if (argc != 1)
    {
        return ctx->asBool(false);
    }
    int button = ctx->getInt(0);

    return ctx->asBool(IsMouseButtonUp(button));
}

static LiteralPtr native_mouse_pressed(ExecutionContext *ctx, int argc)
{
    if (argc != 1)
    {
        return ctx->asBool(false);
    }
    int button = ctx->getInt(0);

    return ctx->asBool(IsMouseButtonPressed(button));
}

static LiteralPtr native_mouse_released(ExecutionContext *ctx, int argc)
{
    if (argc != 1)
    {
        return ctx->asBool(false);
    }
    int button = ctx->getInt(0);

    return ctx->asBool(IsMouseButtonReleased(button));
}

static LiteralPtr native_mouse_x(ExecutionContext *ctx, int argc)
{
    return ctx->asFloat((double)GetMouseX());
}

static LiteralPtr native_mouse_y(ExecutionContext *ctx, int argc)
{
    return ctx->asFloat((double)GetMouseY());
}

static LiteralPtr native_keys_down(ExecutionContext *ctx, int argc)
{
    if (argc != 1)
    {
        ctx->Error("Usage: key_down(id)" + std::to_string(argc));
        return ctx->asBool(false);
    }
    long key = ctx->getInt(0);
    return ctx->asBool(IsKeyDown(key));
}

static LiteralPtr native_keys_up(ExecutionContext *ctx, int argc)
{
    if (argc != 1)
    {
        ctx->Error("Usage: key_up(id)" + std::to_string(argc));
        return ctx->asBool(false);
    }
    long key = ctx->getInt(0);
    return ctx->asBool(IsKeyUp(key));
}

static LiteralPtr native_keys_pressed(ExecutionContext *ctx, int argc)
{
    if (argc != 1)
    {
        ctx->Error("Usage: key_pressed(id)" + std::to_string(argc));
        return ctx->asBool(false);
    }
    long key = ctx->getInt(0);
    return ctx->asBool(IsKeyPressed(key));
}

static LiteralPtr native_keys_released(ExecutionContext *ctx, int argc)
{
    if (argc != 1)
    {
        ctx->Error("Usage: key_released(id)" + std::to_string(argc));
        return ctx->asBool(false);
    }
    long key = ctx->getInt(0);
    return ctx->asBool(IsKeyReleased(key));
}

static LiteralPtr native_keys_key(ExecutionContext *ctx, int argc)
{
    return ctx->asInt(GetKeyPressed());
}

static LiteralPtr native_keys_char(ExecutionContext *ctx, int argc)
{
    return ctx->asInt(GetCharPressed());
}

static const NativeFuncDef native_input_funcs[] =
    {

        {"mouse_down", native_mouse_down},
        {"mouse_up", native_mouse_up},
        {"mouse_released", native_mouse_released},
        {"mouse_pressed", native_mouse_pressed},
        {"mouse_x", native_mouse_x},
        {"mouse_y", native_mouse_y},
        {"key_down", native_keys_down},
        {"key_up", native_keys_up},
        {"key_pressed", native_keys_pressed},
        {"key_released", native_keys_released},
        {"get_key_press", native_keys_key},
        {"get_char_press", native_keys_char},
        {NULL, NULL}};

static LiteralPtr native_circle(ExecutionContext *ctx, int argc)
{

    if (argc != 4)
    {
        ctx->Error("Usage: circle(x, y, radius, fill)" + std::to_string(argc));
        return ctx->asBool(false);
    }
    int x = ctx->getInt(0);
    int y = ctx->getInt(1);
    int radius = ctx->getInt(2);

    bool fill = ctx->getBool(3);

    if (fill)
    {
        DrawCircle(x, y, radius, BLUE);
    }
    else
    {

        DrawCircle(x, y, radius, RED);
    }

    return ctx->asBool(true);
}

static LiteralPtr native_text(ExecutionContext *ctx, int argc)
{

    if (argc != 4)
    {
        ctx->Error("Usage: text(x, y, size, string)" + std::to_string(argc));
        return ctx->asBool(false);
    }
    int x = ctx->getInt(0);
    int y = ctx->getInt(1);
    int radius = ctx->getInt(2);
    std::string text = ctx->getString(3).c_str();

    DrawText(text.c_str(), x, y, radius, RED);

    return ctx->asBool(true);
}

static LiteralPtr native_delta_time(ExecutionContext *ctx, int argc)
{
    if (argc != 0)
    {
        ctx->Error("Usage: delta_time()");
        return ctx->asFloat(0);
    }
    return ctx->asFloat(GetFrameTime());
}

static LiteralPtr native_time(ExecutionContext *ctx, int argc)
{
    if (argc != 0)
    {
        ctx->Error("Usage: Time()");
        return ctx->asFloat(0);
    }
    return ctx->asFloat(GetTime());
}

unsigned int g_seed = 0;

void Random_Seed(const int seed)
{
    if (!seed)
    {
        g_seed = (int)GetTime() * 1000;
    }
    else
    {
        g_seed = seed;
    }
}

long Random_Int(const long min, const long max)
{
    g_seed = 214013 * g_seed + 2531011;
    return min + (g_seed ^ g_seed >> 15) % (max - min + 1);
}

double Random_Float(const double min, const double max)
{
    g_seed = 214013 * g_seed + 2531011;
    // return min+g_seed*(1.0f/4294967295.0f)*(max-min);
    return min + (g_seed >> 16) * (1.0f / 65535.0f) * (max - min);
}

static LiteralPtr native_range(ExecutionContext *ctx, int argc)
{
    if (argc != 2)
    {
        ctx->Error("Usage: range(min,max)");
        return ctx->asFloat(0);
    }
    double min = ctx->getFloat(0);
    double max = ctx->getFloat(1);
    return ctx->asFloat(Random_Float(min, max));
}
static LiteralPtr native_random(ExecutionContext *ctx, int argc)
{
    if (argc != 2)
    {
        ctx->Error("Usage: random(min,max)");
        return ctx->asInt(0);
    }
    long min = ctx->getInt(0);
    long max = ctx->getInt(1);
    return ctx->asInt(Random_Int(min, max));
}

static LiteralPtr native_ping_pong(ExecutionContext *ctx, int argc)
{
    if (argc != 2)
    {
        ctx->Error("Usage: ping_pong( t,  length)");
        return ctx->asFloat(0);
    }
    double t = ctx->getFloat(0);
    double l = ctx->getFloat(1);
    return ctx->asFloat(PingPong(t, l));
}   

static const NativeFuncDef native_core_funcs[] =
    {
        {"Circle", native_circle},
        {"Text", native_text},
        {"DeltaTime", native_delta_time},
        {"Time", native_time},
        {"Range", native_range},
        {"Random", native_random},
        {"PingPong", native_ping_pong},

        {NULL, NULL}};

static LiteralPtr native_advance(ExecutionContext *ctx, int argc)
{
    if (argc != 1)
    {
        ctx->Error("Usage: advance(speed)" + std::to_string(argc));
        return ctx->asBool(false);
    }
    Process *p = ctx->getCurrentProcess();
    double speed = ctx->getFloat(0);
    p->advance(speed);
    return ctx->asBool(true);
}

static LiteralPtr native_xadvance(ExecutionContext *ctx, int argc)
{

    if (argc != 2)
    {
        ctx->Error("Usage: xadvance(speed, angle)" + std::to_string(argc));
        return ctx->asBool(false);
    }
    Process *p = ctx->getCurrentProcess();
    double speed = ctx->getFloat(0);
    double angle = ctx->getFloat(1);
    p->xadvance(speed, angle);
    return ctx->asBool(true);
}

static LiteralPtr native_set_parent(ExecutionContext *ctx, int argc)
{

    if (argc != 0)
    {
        ctx->Error("Usage: set_parent()");
        return ctx->asBool(false);
    }

    // std::string name = ctx->getString(0);

    Process *p = ctx->getCurrentProcess();
    if (p->parent != nullptr)
    {
        // p->parent->instance->setParent( p->instance);
        p->instance->setParent(p->parent->instance);
        return ctx->asBool(true);
    }

    //  Instance *parent =  Scene::Get()->FindInstanceByName(name);
    // if (parent == nullptr)
    // {
    //     ctx->Error("parent not found");
    //     return ctx->asBool(false);
    // }
    // p->instance->setParent(parent->instance);
    return ctx->asBool(false);
}

static LiteralPtr native_center_pivot(ExecutionContext *ctx, int argc)
{

    if (argc != 0)
    {
        ctx->Error("Usage: center_pivot()");
        return ctx->asBool(false);
    }
    Process *p = ctx->getCurrentProcess();
    p->instance->CenterPivot();
    return ctx->asBool(false);
}

static LiteralPtr native_rotate_towards_mouse(ExecutionContext *ctx, int argc)
{

    if (argc != 1)
    {
        ctx->Error("Usage: rotate_towards_mouse(speed)" + std::to_string(argc));
        return ctx->asBool(false);
    }
    Process *p = ctx->getCurrentProcess();
    Vector2 mousePos = GetMousePosition();
    double speed = ctx->getFloat(0);
    double target_angle = atan2(mousePos.y - p->instance->y, mousePos.x - p->instance->x) * 180.0 / M_PI;
    p->rotate_to(-target_angle, speed);

    return ctx->asBool(true);
}


static LiteralPtr native_rotate_towards(ExecutionContext *ctx, int argc)
{

    if (argc != 2)
    {
        ctx->Error("Usage: rotate_towards(process,speed)" + std::to_string(argc));
        return ctx->asBool(false);
    }
    Process *p = ctx->getCurrentProcess();
    std::string name = ctx->getString(0);
    
    Instance *target = Scene::Get().FindInstanceByName(name);
    if (target == nullptr)
    {
        ctx->Error("Process not found");
        return ctx->asBool(false);
    }
    double pos_x = target->x;
    double pos_y = target->y;
    double speed = ctx->getFloat(1);
    double target_angle = -fget_angle(p->instance->x, p->instance->y, pos_x, pos_y);
    p->rotate_to(target_angle, speed);
    return ctx->asBool(true);
}


static LiteralPtr native_get_local_x(ExecutionContext *ctx, int argc)
{

    if (argc != 1)
    {
        ctx->Error("Usage: get_local_x(float)");
        return ctx->asFloat(0);
    }
    Process *p = ctx->getCurrentProcess();
    double value = ctx->getFloat(0);
    Vec2 pos = p->instance->GetLocalPoint(value, 0);
    return ctx->asFloat(pos.x);
}
static LiteralPtr native_get_local_y(ExecutionContext *ctx, int argc)
{

    if (argc != 1)
    {
        ctx->Error("Usage: get_local_y(float)");
        return ctx->asFloat(0);
    }
    Process *p = ctx->getCurrentProcess();
    double value = ctx->getFloat(0);
    Vec2 pos = p->instance->GetLocalPoint(0, value);
    return ctx->asFloat(pos.y);
}

static LiteralPtr native_get_world_y(ExecutionContext *ctx, int argc)
{

    if (argc != 1)
    {
        ctx->Error("Usage: get_world_y(float)");
        return ctx->asFloat(0);
    }
    Process *p = ctx->getCurrentProcess();
    double value = ctx->getFloat(0);
    Vec2 pos = p->instance->GetWorldPoint(value, value);
    return ctx->asFloat(pos.y);
}
static LiteralPtr native_get_world_x(ExecutionContext *ctx, int argc)
{

    if (argc != 1)
    {
        ctx->Error("Usage: get_world_x(float)");
        return ctx->asFloat(0);
    }
    Process *p = ctx->getCurrentProcess();
    double value = ctx->getFloat(0);
    Vec2 pos = p->instance->GetWorldPoint(value, value);
    return ctx->asFloat(pos.x);
}

static LiteralPtr native_get_world_angle(ExecutionContext *ctx, int argc)
{

    if (argc != 0)
    {
        ctx->Error("Usage: get_world_angle()");
        return ctx->asFloat(0);
    }
    Process *p = ctx->getCurrentProcess();

    double angle = p->instance->GetWorldAngle();
    return ctx->asFloat(angle);
}

static LiteralPtr native_in_view(ExecutionContext *ctx, int argc)
{

    if (argc != 0)
    {
        ctx->Error("Usage: in_view()");
        return ctx->asBool(false);
    }
    Process *p = ctx->getCurrentProcess();

    bool result = Scene::Get().InScreen(p->instance);

    return ctx->asBool(result);
}

static LiteralPtr native_out_screen(ExecutionContext *ctx, int argc)
{

    if (argc != 0)
    {
        ctx->Error("Usage: out_screen()");
        return ctx->asBool(false);
    }
    Process *p = ctx->getCurrentProcess();

    bool result = Scene::Get().InScreen(p->instance);

    return ctx->asBool(!result);
}


static LiteralPtr native_place_meeting(ExecutionContext *ctx, int argc)
{

    if (argc != 3)
    {
        ctx->Error("Usage: place_meeting(x, y,process)");
        return ctx->asBool(false);
    }
    Process *p = ctx->getCurrentProcess();
    double x = ctx->getFloat(0);
    double y = ctx->getFloat(1);
    std::string name = ctx->getString(2);
    bool result = p->instance->place_meeting(x, y, name);
    return ctx->asBool(result);
}

static LiteralPtr native_place_free(ExecutionContext *ctx, int argc)
{

    if (argc != 3)
    {
        ctx->Error("Usage: place_free(x, y,process)");
        return ctx->asBool(false);
    }
    Process *p = ctx->getCurrentProcess();
    double x = ctx->getFloat(0);
    double y = ctx->getFloat(1);
    bool result = p->instance->place_free(x, y);
    return ctx->asBool(result);
}




static const NativeFuncDef native_process_funcs[] =
    {

        {"advance", native_advance},
        {"xadvance", native_xadvance},
        {"rotate_towards_mouse", native_rotate_towards_mouse},
        {"rotate_towards", native_rotate_towards},
        {"set_parent", native_set_parent},
        {"center_pivot", native_center_pivot},
        {"get_local_x", native_get_local_x},
        {"get_local_y", native_get_local_y},
        {"get_world_x", native_get_world_x},
        {"get_world_y", native_get_world_y},
        {"get_world_angle", native_get_world_angle},
        {"in_view", native_in_view},
        {"out_screen", native_out_screen},
        {"place_meeting", native_place_meeting},
        {"place_free", native_place_free},

        {NULL, NULL}};

static LiteralPtr native_load_graph(ExecutionContext *ctx, int argc)
{

    if (argc != 2)
    {
        ctx->Error("Usage: load_graph(filename, id)" + std::to_string(argc));
        return ctx->asBool(false);
    }
    std::string filename = ctx->getString(0);
    int id = ctx->getInt(1);
    bool result = Scene::Get().addGraphFromFile(filename, id);
    return ctx->asBool(result);
}

static LiteralPtr native_load_atlas(ExecutionContext *ctx, int argc)
{

    if (argc != 4)
    {
        ctx->Error("Usage: load_atlas(filename, id, width, height)" + std::to_string(argc));
        return ctx->asBool(false);
    }
    std::string filename = ctx->getString(0);
    int id = ctx->getInt(1);
    int width = ctx->getInt(2);
    int height = ctx->getInt(3);
    

    bool result = Scene::Get().addAtlasGraphFromFile(filename, id, width, height);
    return ctx->asBool(result);
}

static const NativeFuncDef native_scene_funcs[] =
    {

        {"load_graph", native_load_graph},
        {"load_atlas", native_load_atlas},

        {NULL, NULL}};

static void global_scope(ExecutionContext *ctx)
{
    ctx->define_int("_left", KEY_LEFT);
    ctx->define_int("_right", KEY_RIGHT);
    ctx->define_int("_up", KEY_UP);
    ctx->define_int("_down", KEY_DOWN);
    ctx->define_int("_space", KEY_SPACE);
    ctx->define_int("_enter", KEY_ENTER);
    ctx->define_int("_backspace", KEY_BACKSPACE);
    ctx->define_int("_delete", KEY_DELETE);

    ctx->define_int("_a", KEY_A);
    ctx->define_int("_b", KEY_B);
    ctx->define_int("_c", KEY_C);
    ctx->define_int("_d", KEY_D);
    ctx->define_int("_e", KEY_E);
    ctx->define_int("_f", KEY_F);
    ctx->define_int("_g", KEY_G);
    ctx->define_int("_h", KEY_H);
    ctx->define_int("_i", KEY_I);
    ctx->define_int("_j", KEY_J);
    ctx->define_int("_k", KEY_K);
    ctx->define_int("_l", KEY_L);
    ctx->define_int("_m", KEY_M);
    ctx->define_int("_n", KEY_N);
    ctx->define_int("_o", KEY_O);
    ctx->define_int("_p", KEY_P);
    ctx->define_int("_q", KEY_Q);
    ctx->define_int("_r", KEY_R);
    ctx->define_int("_s", KEY_S);
    ctx->define_int("_t", KEY_T);
    ctx->define_int("_u", KEY_U);
    ctx->define_int("_v", KEY_V);
    ctx->define_int("_w", KEY_W);
    ctx->define_int("_x", KEY_X);
    ctx->define_int("_y", KEY_Y);
    ctx->define_int("_z", KEY_Z);

    ctx->define_int("_1", KEY_ONE);
    ctx->define_int("_2", KEY_TWO);
    ctx->define_int("_3", KEY_THREE);
    ctx->define_int("_4", KEY_FOUR);
    ctx->define_int("_5", KEY_FIVE);
    ctx->define_int("_6", KEY_SIX);
    ctx->define_int("_7", KEY_SEVEN);
    ctx->define_int("_8", KEY_EIGHT);
    ctx->define_int("_9", KEY_NINE);
    ctx->define_int("_0", KEY_ZERO);
}

void register_core(Interpreter *interpreter)
{
    Random_Seed(0);
    for (const NativeFuncDef *def = native_core_funcs; def->name != NULL; def++)
    {
        interpreter->registerFunction(def->name, def->func);
    }

    for (const NativeFuncDef *def = native_input_funcs; def->name != NULL; def++)
    {
        interpreter->registerFunction(def->name, def->func);
    }

    for (const NativeFuncDef *def = native_process_funcs; def->name != NULL; def++)
    {
        interpreter->registerFunction(def->name, def->func);
    }
    for (const NativeFuncDef *def = native_scene_funcs; def->name != NULL; def++)
    {
        interpreter->registerFunction(def->name, def->func);
    }

    interpreter->registerGlobalScope(global_scope);
}
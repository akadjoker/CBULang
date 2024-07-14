#pragma once

#include <math.h>

#include <raylib.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <raylib.h>

#define BLEND_ALPHAMULT 6
#define BLEND_ALPHABLEND 2
#define BLEND_ALPHAADD 0

#define BLEND_ZWRITE 4
#define BLEND_NOZWRITE 0

#define BLEND_DEFAULT (BLEND_ALPHABLEND | BLEND_NOZWRITE)
#define BLEND_DEFAULT_Z (BLEND_ALPHABLEND | BLEND_ZWRITE)

const bool FIX_ARTIFACTS_BY_STRECHING_TEXEL = true;

struct rVertex
{
    float x, y, z;
    Color col;
    float tx, ty;
};

struct rQuad
{
    rVertex v[4];
    Texture2D tex;
    int blend;
};
//************************************************************/
//********************** VEC2" ******************************/
//************************************************************/

struct Vec2
{
    float x;
    float y;

    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float xy) : x(xy), y(xy) {}
    Vec2(float x, float y) : x(x), y(y) {}

    Vec2 &add(const Vec2 &other);
    Vec2 &subtract(const Vec2 &other);
    Vec2 &multiply(const Vec2 &other);
    Vec2 &divide(const Vec2 &other);

    Vec2 &add(float value);
    Vec2 &subtract(float value);
    Vec2 &multiply(float value);
    Vec2 &divide(float value);

    friend Vec2 operator+(Vec2 left, const Vec2 &right);
    friend Vec2 operator-(Vec2 left, const Vec2 &right);
    friend Vec2 operator*(Vec2 left, const Vec2 &right);
    friend Vec2 operator/(Vec2 left, const Vec2 &right);

    friend Vec2 operator+(Vec2 left, float value);
    friend Vec2 operator-(Vec2 left, float value);
    friend Vec2 operator*(Vec2 left, float value);
    friend Vec2 operator/(Vec2 left, float value);

    bool operator==(const Vec2 &other) const;
    bool operator!=(const Vec2 &other) const;

    Vec2 &operator+=(const Vec2 &other);
    Vec2 &operator-=(const Vec2 &other);
    Vec2 &operator*=(const Vec2 &other);
    Vec2 &operator/=(const Vec2 &other);

    Vec2 &operator+=(float value);
    Vec2 &operator-=(float value);
    Vec2 &operator*=(float value);
    Vec2 &operator/=(float value);

    bool operator<(const Vec2 &other) const;
    bool operator<=(const Vec2 &other) const;
    bool operator>(const Vec2 &other) const;
    bool operator>=(const Vec2 &other) const;

    float magnitude() const;
    Vec2 normalised() const;
    float distance(const Vec2 &other) const;
    float dot(const Vec2 &other) const;
};

class Matrix2D
{
public:
    Matrix2D();
    virtual ~Matrix2D();
    void Identity();
    void Set(float a, float b, float c, float d, float tx, float ty);
    void Concat(const Matrix2D &m);
    Vec2 TransformCoords(Vec2 point);
    Matrix2D Mult(const Matrix2D &m);
    void Rotate(float angle);
    void Scale(float x, float y);
    void Translate(float x, float y);
    void Skew(float skewX, float skewY);

public:
    float a;
    float b;
    float c;
    float d;
    float tx;
    float ty;
};

using ComponentID = size_t;
ComponentID GetUniqueComponentID() noexcept;

class Scene;
class Instance;

class Graph
{
public:
    Graph()
    {
        id = -1;
        texture = {0};
        image = {0};
        isClip = false;
    };

    inline int GetWidth() const { return texture.width; }
    inline int GetHeight() const { return texture.height; }
    std::vector<Vec2> points;
    std::string name;
    Texture2D texture;
    Image image;
    bool isClip;
    Rectangle clip;
    int id;
};

enum InstanceType
{
    ISprite,
    ITileMap
};

class Mask
{
public:
    Mask()
    {
        parent = nullptr;
    }
    virtual ~Mask()
    {
        parent = nullptr;
    }
    virtual bool collide(const Mask *other);
    virtual bool collideMask(const Mask *other);

    Instance *parent;
};

class Hitbox : public Mask
{
public:
    Hitbox(int width = 1, int height = 1, int x = 0, int y = 0) : Mask()
    {
        this->width = width;
        this->height = height;
        this->x = x;
        this->y = y;
    }
    virtual ~Hitbox()
    {
    }

    bool collideMask(const Mask *other);
    bool collideHitbox(const Hitbox *other);

    int width;
    int height;
    int x;
    int y;
};

//************************************************************/
//********************** MATRIX2D ****************************/
//************************************************************/

class Instance
{
private:
    long m_id;
    friend class Scene;

protected:

    std::vector<Instance *> m_colide_entities;

public:
    virtual ~Instance();
    Instance();

    std::string name;

    int width;
    int height;
    int originX;
    int originY;
    Mask *mask{nullptr};

    // properties
    bool collidable;
    Color color;
    Vec2 scale;
    Vec2 size;
    Vec2 pivot;
    Vec2 skew;

    Matrix2D matrix;
    Matrix2D local_transform;
    Matrix2D wordl_transform;
    bool alive{true};
    double x{0};
    double y{0};
    double angle{0};
    long id;
    int graph{0};
    bool solid{false};
    bool visible{true};
    bool active{true};
    bool flip_x{false};
    bool flip_y{false};
    int layer{1};
    bool showBox{false};
    bool showPivot{false};
    double _x{0};
    double _y{0};
    Instance *m_parent{nullptr};

    void Destroy();

    void Update(double delta);
    void Render();

    void setGraph(int id);

    bool collideWith(const Instance *e, double x, double y);
    bool place_meeting(double x, double y, std::string &name);
    bool place_free(double x, double y);
    Instance *collide(double x, double y);
    bool collideWithRect(const Rectangle e, double x, double y, int Pivot_x, int Pivot_y);

    void Kill();

    double getWorldX() const;
    double getWorldY() const;
    double getWorldOriginX() const;
    double getWorldOriginY() const;
    double GetWorldAngle();

    void setHitbox(int width, int height, int originX = 0, int originY = 0);
    void centerOrigin();
    void setOrigin(int _x, int _y);

    void setPivot(double x, double y);
    void CenterPivot();
    void setParent(Instance *parent);



    Matrix2D GetLocalTrasformation();
    Matrix2D GetWorldTransformation();

    Vec2 get_point(size_t index);
    Vec2 get_real_point(size_t index);
    Vec2 GetPoint(double _x, double _y);
    Vec2 GetWorldPoint(double _x, double _y);
    Vec2 GetWorldPoint(Vec2 p);
    Vec2 GetLocalPoint(double _x, double y);
    Vec2 GetLocalPoint(Vec2 p);

    bool operator==(const Instance &other) const
    {
        return (m_id == other.m_id);
    }

    bool operator!=(const Instance &other) const
    {
        return !(*this == other);
    }
};

//*************************************************************/
//********************** SCENE ****************************/
//************************************************************/

class Scene
{
private:
    std::vector<Instance *> m_entities;
    std::unordered_map<int, std::vector<Instance *>> m_layers;
    int m_num_layers{0};

    Color clearColor;
    Camera2D camera{0};
    Vec2 offset;    // Camera offset (displacement from target)
    Vec2 target;    // Camera target (rotation and zoom origin)
    float rotation; // Camera rotation in degrees
    float zoom;     // Camera zoom (scaling), should be 1.0f by default
    Rectangle bound;


    Scene();
    virtual ~Scene();

public:
    static Scene &Get()
    {
        static Scene scene;
        return scene;
    }
    void Refresh();

    void Update(float delta);
    void UpdateAndRefresh();

    void UpdateBehaviourInstances();

    // clear the scene via assync
    void CleanAndRemove();
    // clear all death object
    void Clear();

    void message(int x, int y, Color c, const char *text, ...);

    // add object to layer
    void addToLayer(Instance *e);

    // create layers
    int addLayer();
    int addLayers(int count);
    int layersCount();

    Instance *CreateInstance(long ID, const std::string &name, int graph, double x, double y, double angle, int layer);

    Instance *FindInstanceByName(const std::string &name);

    std::unordered_map<int, Graph> graphics;

    std::unordered_map<std::string, Texture2D> texture_list;
    std::string gameName;

    bool addGraphFromFile(const std::string &string, int id);
    int loadGraphFromFile(const std::string &string);

    bool addSubGraphFromFile(int graph, int id, int x, int y, int w, int h);
    bool addAtlasGraphFromFile(const std::string &string, int id, int w, int h);

    Graph getGraph(int id);
    bool graphExists(int id);
    bool graph_add_point(int id, double x, double y);
    int  graph_total_points(int id);

    int getGraphWidth(int value);
    int getGraphHeight(int value);

    void setCameraPosition(float x, float y);
    void setCameraZoom(float value);

    void setClearColor(int r, int g, int b);

    bool InScreen(Instance *e);

    const std::vector<Instance *> &GetEntities() { return m_entities; }
    const std::vector<Instance *> &GetLayerEntities(int layer) { return m_layers[layer]; }
};

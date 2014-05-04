#include "blockrenderer.h"

#include "billboardrenderer.h"
#include "cakerenderer.h"
#include "doorrenderer.h"
#include "glassrenderer.h"
#include "leavesrenderer.h"
#include "torchrenderer.h"

UniversalBlockRenderer global_block_renderer;

void BlockRenderer::renderNormalBlockSide(int local_x, int local_y, int local_z, const BLOCK_SIDE side, const TextureAtlasEntry tex, Chunk &c, const COLOR color)
{
    switch(side)
    {
    case BLOCK_FRONT:
        c.addAlignedVertex(local_x, local_y, local_z, tex.left, tex.bottom, color);
        c.addAlignedVertex(local_x, local_y + 1, local_z, tex.left, tex.top, color);
        c.addAlignedVertex(local_x + 1, local_y + 1, local_z, tex.right, tex.top, color);
        c.addAlignedVertex(local_x + 1, local_y, local_z, tex.right, tex.bottom, color);
        return;
    case BLOCK_BACK:
        c.addAlignedVertex(local_x + 1, local_y, local_z + 1, tex.left, tex.bottom, color);
        c.addAlignedVertex(local_x + 1, local_y + 1, local_z + 1, tex.left, tex.top, color);
        c.addAlignedVertex(local_x, local_y + 1, local_z + 1, tex.right, tex.top, color);
        c.addAlignedVertex(local_x, local_y, local_z + 1, tex.right, tex.bottom, color);
        return;
    case BLOCK_RIGHT:
        c.addAlignedVertex(local_x + 1, local_y, local_z, tex.right, tex.bottom, color);
        c.addAlignedVertex(local_x + 1, local_y + 1, local_z, tex.right, tex.top, color);
        c.addAlignedVertex(local_x + 1, local_y + 1, local_z + 1, tex.left, tex.top, color);
        c.addAlignedVertex(local_x + 1, local_y, local_z + 1, tex.left, tex.bottom, color);
        return;
    case BLOCK_LEFT:
        c.addAlignedVertex(local_x, local_y, local_z + 1, tex.left, tex.bottom, color);
        c.addAlignedVertex(local_x, local_y + 1, local_z + 1, tex.left, tex.top, color);
        c.addAlignedVertex(local_x, local_y + 1, local_z, tex.right, tex.top, color);
        c.addAlignedVertex(local_x, local_y, local_z, tex.right, tex.bottom, color);
        return;
    case BLOCK_TOP:
        c.addAlignedVertex(local_x, local_y + 1, local_z, tex.left, tex.bottom, color);
        c.addAlignedVertex(local_x, local_y + 1, local_z + 1, tex.left, tex.top, color);
        c.addAlignedVertex(local_x + 1, local_y + 1, local_z + 1, tex.right, tex.top, color);
        c.addAlignedVertex(local_x + 1, local_y + 1, local_z, tex.right, tex.bottom, color);
        return;
    case BLOCK_BOTTOM:
        c.addAlignedVertex(local_x + 1, local_y, local_z, tex.left, tex.bottom, color);
        c.addAlignedVertex(local_x + 1, local_y, local_z + 1, tex.left, tex.top, color);
        c.addAlignedVertex(local_x, local_y, local_z + 1, tex.right, tex.top, color);
        c.addAlignedVertex(local_x, local_y, local_z, tex.right, tex.bottom, color);
        return;
    default:
        return; //WTF
    }
}

void BlockRenderer::renderBillboard(int local_x, int local_y, int local_z, const TextureAtlasEntry tex, Chunk &c)
{
    //| 0xFFF = no backface culling
    c.addAlignedVertex(local_x, local_y, local_z, tex.left, tex.bottom, TEXTURE_TRANSPARENT | 0xFFF);
    c.addAlignedVertex(local_x, local_y + 1, local_z, tex.left, tex.top, TEXTURE_TRANSPARENT | 0xFFF);
    c.addAlignedVertex(local_x + 1, local_y + 1, local_z + 1, tex.right, tex.top, TEXTURE_TRANSPARENT | 0xFFF);
    c.addAlignedVertex(local_x + 1, local_y, local_z + 1, tex.right, tex.bottom, TEXTURE_TRANSPARENT | 0xFFF);

    c.addAlignedVertex(local_x, local_y, local_z + 1, tex.left, tex.bottom, TEXTURE_TRANSPARENT | 0xFFF);
    c.addAlignedVertex(local_x, local_y + 1, local_z + 1, tex.left, tex.top, TEXTURE_TRANSPARENT | 0xFFF);
    c.addAlignedVertex(local_x + 1, local_y + 1, local_z, tex.right, tex.top, TEXTURE_TRANSPARENT | 0xFFF);
    c.addAlignedVertex(local_x + 1, local_y, local_z, tex.right, tex.bottom, TEXTURE_TRANSPARENT | 0xFFF);
}

void BlockRenderer::drawTextureAtlasEntry(TEXTURE &src, const TextureAtlasEntry &tex, bool transparent, TEXTURE &dest, const int dest_x, const int dest_y)
{
    if(transparent)
        drawTransparentTexture(src, tex.left, tex.top, dest, dest_x, dest_y, tex.right - tex.left, tex.bottom - tex.top);
    else
        drawTexture(src, tex.left, tex.top, dest, dest_x, dest_y, tex.right - tex.left, tex.bottom - tex.top);
}

UniversalBlockRenderer::UniversalBlockRenderer()
{
    auto normal_renderer = std::make_shared<NormalBlockRenderer>();
    auto oriented_renderer = std::make_shared<OrientedBlockRenderer>();
    auto null_renderer = std::make_shared<NullBlockRenderer>();

    BLOCK i = 1;
    for(; i <= BLOCK_NORMAL_LAST; ++i)
        map[i] = normal_renderer;

    while(true)
    {
        map[i] = null_renderer;
        if(i == 255)
            break;

        i++;
    }

    map[BLOCK_AIR] = null_renderer;
    map[BLOCK_BOOKSHELF] = oriented_renderer;
    map[BLOCK_CAKE] = std::make_shared<CakeRenderer>();
    map[BLOCK_CRAFTING_TABLE] = oriented_renderer;
    map[BLOCK_DOOR] = std::make_shared<DoorRenderer>();
    map[BLOCK_FURNACE] = oriented_renderer;
    map[BLOCK_GLASS] = std::make_shared<GlassRenderer>();
    map[BLOCK_LEAVES] = std::make_shared<LeavesRenderer>();
    map[BLOCK_PUMPKIN] = oriented_renderer;
    map[BLOCK_TORCH] = std::make_shared<TorchRenderer>();

    auto flower_renderer = std::make_shared<BillboardRenderer>();
    flower_renderer->setEntry(0, 12, 0, "Red flower", BLOCK_SIZE, BLOCK_SIZE/2, BLOCK_SIZE);
    flower_renderer->setEntry(1, 13, 0, "Yellow flower", BLOCK_SIZE, BLOCK_SIZE/2, BLOCK_SIZE);
    map[BLOCK_FLOWER] = flower_renderer;

    auto mushroom_renderer = std::make_shared<BillboardRenderer>();
    mushroom_renderer->setEntry(0, 12, 1, "Red mushroom", BLOCK_SIZE/3, BLOCK_SIZE/3, BLOCK_SIZE/3);
    mushroom_renderer->setEntry(1, 13, 1, "Grey mushroom", BLOCK_SIZE/3, BLOCK_SIZE/3, BLOCK_SIZE/3);
    map[BLOCK_MUSHROOM] = mushroom_renderer;

    auto spiderweb_renderer = std::make_shared<BillboardRenderer>();
    spiderweb_renderer->setEntry(0, 11, 0, "Spiderweb", BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
    map[BLOCK_SPIDERWEB] = spiderweb_renderer;

#ifdef DEBUG
    puts("UniversalBlockRenderer created.");
#endif
}

UniversalBlockRenderer::~UniversalBlockRenderer()
{
#ifdef DEBUG
    puts("UniversalBlockRenderer destroyed.");
#endif
}

void UniversalBlockRenderer::renderSpecialBlock(const BLOCK_WDATA block, GLFix x, GLFix y, GLFix z, Chunk &c)
{
    return map[getBLOCK(block)]->renderSpecialBlock(block, x, y, z, c);
}

int UniversalBlockRenderer::indicesNormalBlock(const BLOCK_WDATA block, const int local_x, const int local_y, const int local_z, const BLOCK_SIDE side, Chunk &c)
{
    return map[getBLOCK(block)]->indicesNormalBlock(block, local_x, local_y, local_z, side, c);
}

void UniversalBlockRenderer::geometryNormalBlock(const BLOCK_WDATA block, const int local_x, const int local_y, const int local_z, const BLOCK_SIDE side, Chunk &c)
{
    return map[getBLOCK(block)]->geometryNormalBlock(block, local_x, local_y, local_z, side, c);
}

bool UniversalBlockRenderer::isOpaque(const BLOCK_WDATA block)
{
    return map[getBLOCK(block)]->isOpaque(block);
}

bool UniversalBlockRenderer::isObstacle(const BLOCK_WDATA block)
{
    return map[getBLOCK(block)]->isObstacle(block);
}

bool UniversalBlockRenderer::isOriented(const BLOCK_WDATA block)
{
    return map[getBLOCK(block)]->isOriented(block);
}

bool UniversalBlockRenderer::isFullyOriented(const BLOCK_WDATA block)
{
    return map[getBLOCK(block)]->isFullyOriented(block);
}

bool UniversalBlockRenderer::isBlockShaped(const BLOCK_WDATA block)
{
    return map[getBLOCK(block)]->isBlockShaped(block);
}

AABB UniversalBlockRenderer::getAABB(const BLOCK_WDATA block, GLFix x, GLFix y, GLFix z)
{
    return map[getBLOCK(block)]->getAABB(block, x, y, z);
}

void UniversalBlockRenderer::drawPreview(const BLOCK_WDATA block, TEXTURE &dest, const int x, const int y)
{
    return map[getBLOCK(block)]->drawPreview(block, dest, x, y);
}

bool UniversalBlockRenderer::action(const BLOCK_WDATA block, const int local_x, const int local_y, const int local_z, Chunk &c)
{
    return map[getBLOCK(block)]->action(block, local_x, local_y, local_z, c);
}

void UniversalBlockRenderer::tick(const BLOCK_WDATA block, int local_x, int local_y, int local_z, Chunk &c)
{
    return map[getBLOCK(block)]->tick(block, local_x, local_y, local_z, c);
}

void UniversalBlockRenderer::updateBlock(const BLOCK_WDATA block, int local_x, int local_y, int local_z, Chunk &c)
{
    return map[getBLOCK(block)]->updateBlock(block, local_x, local_y, local_z, c);
}

void UniversalBlockRenderer::addedBlock(const BLOCK_WDATA block, int local_x, int local_y, int local_z, Chunk &c)
{
    return map[getBLOCK(block)]->addedBlock(block, local_x, local_y, local_z, c);
}

void UniversalBlockRenderer::removedBlock(const BLOCK_WDATA block, int local_x, int local_y, int local_z, Chunk &c)
{
    return map[getBLOCK(block)]->removedBlock(block, local_x, local_y, local_z, c);
}

const char *UniversalBlockRenderer::getName(const BLOCK_WDATA block)
{
    return map[getBLOCK(block)]->getName(block);
}

void NormalBlockRenderer::geometryNormalBlock(const BLOCK_WDATA block, const int local_x, const int local_y, const int local_z, const BLOCK_SIDE side, Chunk &c)
{
    BlockRenderer::renderNormalBlockSide(local_x, local_y, local_z, side, block_textures[getBLOCK(block)][side].current, c);
}

void NormalBlockRenderer::drawPreview(const BLOCK_WDATA block, TEXTURE &dest, const int dest_x, const int dest_y)
{
    const TextureAtlasEntry tex = block_textures[getBLOCK(block)][BLOCK_FRONT].resized;

    BlockRenderer::drawTextureAtlasEntry(*terrain_resized, tex, false, dest, dest_x, dest_y);
}

void OrientedBlockRenderer::geometryNormalBlock(const BLOCK_WDATA block, int local_x, int local_y, int local_z, const BLOCK_SIDE side, Chunk &c)
{
    BLOCK_SIDE map[BLOCK_SIDE_LAST + 1];

    BLOCK type = getBLOCK(block);
    switch(static_cast<BLOCK_SIDE>(getBLOCKDATA(block)))
    {
    default:
    case BLOCK_FRONT:
        map[BLOCK_TOP] = BLOCK_TOP;
        map[BLOCK_BOTTOM] = BLOCK_BOTTOM;
        map[BLOCK_LEFT] = BLOCK_LEFT;
        map[BLOCK_RIGHT] = BLOCK_RIGHT;
        map[BLOCK_BACK] = BLOCK_BACK;
        map[BLOCK_FRONT] = BLOCK_FRONT;
        break;
    case BLOCK_BACK:
        map[BLOCK_TOP] = BLOCK_TOP;
        map[BLOCK_BOTTOM] = BLOCK_BOTTOM;
        map[BLOCK_LEFT] = BLOCK_RIGHT;
        map[BLOCK_RIGHT] = BLOCK_LEFT;
        map[BLOCK_BACK] = BLOCK_FRONT;
        map[BLOCK_FRONT] = BLOCK_BACK;
        break;
    case BLOCK_LEFT:
        map[BLOCK_TOP] = BLOCK_TOP;
        map[BLOCK_BOTTOM] = BLOCK_BOTTOM;
        map[BLOCK_LEFT] = BLOCK_FRONT;
        map[BLOCK_RIGHT] = BLOCK_BACK;
        map[BLOCK_BACK] = BLOCK_LEFT;
        map[BLOCK_FRONT] = BLOCK_RIGHT;
        break;
    case BLOCK_RIGHT:
        map[BLOCK_TOP] = BLOCK_TOP;
        map[BLOCK_BOTTOM] = BLOCK_BOTTOM;
        map[BLOCK_LEFT] = BLOCK_BACK;
        map[BLOCK_RIGHT] = BLOCK_FRONT;
        map[BLOCK_BACK] = BLOCK_RIGHT;
        map[BLOCK_FRONT] = BLOCK_LEFT;
        break;
    }

    BlockRenderer::renderNormalBlockSide(local_x, local_y, local_z, side, block_textures[type][map[side]].current, c);
}
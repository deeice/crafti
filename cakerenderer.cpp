#include "cakerenderer.h"

void CakeRenderer::renderSpecialBlock(const BLOCK_WDATA /*block*/, GLFix x, GLFix y, GLFix z, Chunk &c)
{
    const GLFix cake_offset = (GLFix(BLOCK_SIZE) - cake_width) * GLFix(0.5f);
    const TextureAtlasEntry &cake_top = terrain_atlas[9][7].current;
    const TextureAtlasEntry &cake_sid = terrain_atlas[10][7].current;

    c.addUnalignedVertex({x, y, z + cake_offset, cake_sid.left, cake_sid.bottom, TEXTURE_TRANSPARENT});
    c.addUnalignedVertex({x, y + BLOCK_SIZE, z + cake_offset, cake_sid.left, cake_sid.top, TEXTURE_TRANSPARENT});
    c.addUnalignedVertex({x + BLOCK_SIZE, y + BLOCK_SIZE, z + cake_offset, cake_sid.right, cake_sid.top, TEXTURE_TRANSPARENT});
    c.addUnalignedVertex({x + BLOCK_SIZE, y, z + cake_offset, cake_sid.right, cake_sid.bottom, TEXTURE_TRANSPARENT});

    c.addUnalignedVertex({x + BLOCK_SIZE, y, z - cake_offset + BLOCK_SIZE, cake_sid.left, cake_sid.bottom, TEXTURE_TRANSPARENT});
    c.addUnalignedVertex({x + BLOCK_SIZE, y + BLOCK_SIZE, z - cake_offset + BLOCK_SIZE, cake_sid.left, cake_sid.top, TEXTURE_TRANSPARENT});
    c.addUnalignedVertex({x, y + BLOCK_SIZE, z - cake_offset + BLOCK_SIZE, cake_sid.right, cake_sid.top, TEXTURE_TRANSPARENT});
    c.addUnalignedVertex({x, y, z - cake_offset + BLOCK_SIZE, cake_sid.right, cake_sid.bottom, TEXTURE_TRANSPARENT});

    c.addUnalignedVertex({x + cake_offset, y, z + BLOCK_SIZE, cake_sid.left, cake_sid.bottom, TEXTURE_TRANSPARENT});
    c.addUnalignedVertex({x + cake_offset, y + BLOCK_SIZE, z + BLOCK_SIZE, cake_sid.left, cake_sid.top, TEXTURE_TRANSPARENT});
    c.addUnalignedVertex({x + cake_offset, y + BLOCK_SIZE, z, cake_sid.right, cake_sid.top, TEXTURE_TRANSPARENT});
    c.addUnalignedVertex({x + cake_offset, y, z, cake_sid.right, cake_sid.bottom, TEXTURE_TRANSPARENT});

    c.addUnalignedVertex({x - cake_offset + BLOCK_SIZE, y, z, cake_sid.left, cake_sid.bottom, TEXTURE_TRANSPARENT});
    c.addUnalignedVertex({x - cake_offset + BLOCK_SIZE, y + BLOCK_SIZE, z, cake_sid.left, cake_sid.top, TEXTURE_TRANSPARENT});
    c.addUnalignedVertex({x - cake_offset + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE, cake_sid.right, cake_sid.top, TEXTURE_TRANSPARENT});
    c.addUnalignedVertex({x - cake_offset + BLOCK_SIZE, y, z + BLOCK_SIZE, cake_sid.right, cake_sid.bottom, TEXTURE_TRANSPARENT});

    c.addUnalignedVertex({x, y + cake_height, z, cake_top.left, cake_top.bottom, TEXTURE_TRANSPARENT});
    c.addUnalignedVertex({x, y + cake_height, z + BLOCK_SIZE, cake_top.left, cake_top.top, TEXTURE_TRANSPARENT});
    c.addUnalignedVertex({x + BLOCK_SIZE, y + cake_height, z + BLOCK_SIZE, cake_top.right, cake_top.top, TEXTURE_TRANSPARENT});
    c.addUnalignedVertex({x + BLOCK_SIZE, y + cake_height, z, cake_top.right, cake_top.bottom, TEXTURE_TRANSPARENT});
}

void CakeRenderer::geometryNormalBlock(const BLOCK_WDATA /*block*/, const int local_x, const int local_y, const int local_z, const BLOCK_SIDE side, Chunk &c)
{
    if(side != BLOCK_BOTTOM)
        return;

    renderNormalBlockSide(local_x, local_y, local_z, side, terrain_atlas[12][7].current, c);
}

AABB CakeRenderer::getAABB(const BLOCK_WDATA /*block*/, GLFix x, GLFix y, GLFix z)
{
    const GLFix cake_offset = (GLFix(BLOCK_SIZE) - cake_width) * GLFix(0.5f);

    return {x + cake_offset, y, z + cake_offset, x + cake_offset + cake_width, y + cake_height, z + cake_offset + cake_width};
}

void CakeRenderer::drawPreview(const BLOCK_WDATA /*block*/, TEXTURE &dest, int x, int y)
{
    TextureAtlasEntry &tex = terrain_atlas[12][8].resized;
    BlockRenderer::drawTextureAtlasEntry(*terrain_resized, tex, dest, x, y);
}

const char *CakeRenderer::getName(const BLOCK_WDATA /*block*/)
{
    return "Cake";
}

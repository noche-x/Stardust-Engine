#include <Graphics/2D/TilemapAnim.h>

namespace Stardust::Graphics::Render2D {
	TilemapAnim::TilemapAnim(TextureAtlas* atlas, Texture* texture)
	{
		m_Atlas = atlas;
		texRef = texture;

		vertices.clear();
		m_Tiles.clear();
		position = { 0, 0 };
	}
	TilemapAnim::~TilemapAnim()
	{
		clearTiles();
	}
	int TilemapAnim::addTile(TileAnim* t)
	{
		int idx = m_Tiles.size();
		m_Tiles.push_back(t);
		return idx;
	}
	void TilemapAnim::updateTile(int tileNum, TileAnim* t)
	{
		delete m_Tiles[tileNum];
		m_Tiles[tileNum] = t;
	}

	void TilemapAnim::setAmbientLight(AmbientLight light)
	{
		ambient = light;
	}

	void TilemapAnim::addPointLight(PointLight light)
	{
		pointLights.push_back(light);
	}

	void TilemapAnim::calculateLighting()
	{
		vertices.clear();

		for (auto t : m_Tiles) {
			uint8_t r = (float)ambient.r * ambient.intensity;
			uint8_t g = (float)ambient.g * ambient.intensity;
			uint8_t b = (float)ambient.b * ambient.intensity;

			for (auto p : pointLights) {
				float distance = sqrtf((p.x - t->offset.x) * (p.x - t->offset.x) + (p.y - t->offset.y) * (p.y - t->offset.y));
				float corrRange = (p.range - distance) / p.range;

				float intensityMax = 1.0f - ambient.intensity;

				float intensity = 0.0f;
				if (p.intensity > intensityMax) {
					intensity = intensityMax;
				}
				else {
					intensity = p.intensity;
				}

				if (corrRange > 1.0f) {
					corrRange = 1.0f;
				}

				if (corrRange > 0.0f) {
					r += ((float)p.r) * intensity * corrRange;
					g += ((float)p.g) * intensity * corrRange;
					b += ((float)p.b) * intensity * corrRange;
				}

				if (r > 255) {
					r = 255;
				}
				if (g > 255) {
					g = 255;
				}
				if (b > 255) {
					b = 255;
				}

				if (r < 0) {
					r = 0;
				}
				if (g < 0) {
					g = 0;
				}
				if (b < 0) {
					b = 0;
				}
			}

			t->rgba = GU_RGBA(r, g, b, 255);
		}

		buildMap();
	}

	void TilemapAnim::tickPhase()
	{
		for (int i = 0; i < m_Tiles.size(); i++) {
			auto t = m_Tiles[i];

			if (!t->isAnim)
				continue;

			t->tickNumber++;
			if (t->tickNumber > t->animLength) {
				t->tickNumber = 0;
			}

			std::array<float, 8> coords = m_Atlas->getTexture(t->indexStart + t->tickNumber);

			if (t->rotation == 0 || t->rotation == 4) {
				vertices[i*2] = { coords[(0 + t->rotation) % 8], coords[(1 + t->rotation) % 8], t->rgba, t->offset.x, t->offset.y , t->layer };
				vertices[i*2 + 1] = { coords[(4 + t->rotation) % 8], coords[(5 + t->rotation) % 8], t->rgba, t->offset.x + t->extent.x, t->offset.y + t->extent.y, t->layer };
			}
			else {
				vertices[i * 2] = { coords[(0 + t->rotation) % 8], coords[(1 + t->rotation) % 8], t->rgba, t->offset.x + t->extent.x, t->offset.y , t->layer };
				vertices[i * 2 + 1] = { coords[(4 + t->rotation) % 8], coords[(5 + t->rotation) % 8], t->rgba, t->offset.x, t->offset.y + t->extent.y, t->layer };
			}

		}
		sceKernelDcacheWritebackInvalidateAll();
	}


	bool TilemapAnim::checkPhysics(const Math::AABB2D& aabb)
	{
		for (auto t : m_Tiles) {

			if (t->physics) {
				//Stardust::Utilities::app_Logger->log("POS: " + std::to_string(aabb.offset.x) + " " + std::to_string(aabb.offset.y));
				Math::AABB2D b = { {t->offset.x, t->offset.y + 272}, t->extent };
				b.offset += position;
				//Stardust::Utilities::app_Logger->log("COLLIDER: " + std::to_string(b.offset.x) + " " + std::to_string(b.offset.y));
				bool res = Math::AABBIntersect2D(aabb, b);

				if (res)
					return res;
			}
		}
		return false;
	}

	void TilemapAnim::clearTiles()
	{
		for (auto t : m_Tiles) {
			delete t;
		}
		m_Tiles.clear();
		vertices.clear();
	}
	void TilemapAnim::setPosition(glm::vec2 pos)
	{
		position = pos;
	}
	void TilemapAnim::buildMap()
	{
		vertices.clear();
		for (auto t : m_Tiles) {
			std::array<float, 8> coords;
			if (t->isAnim) {
				coords = m_Atlas->getTexture(t->indexStart + t->tickNumber);
			}
			else {
				coords = m_Atlas->getTexture(t->texIndex);
			}

			if (t->rotation == 0 || t->rotation == 4) {
				vertices.push_back({ coords[(0 + t->rotation) % 8], coords[(1 + t->rotation) % 8], t->rgba, t->offset.x, t->offset.y , t->layer });
				vertices.push_back({ coords[(4 + t->rotation) % 8], coords[(5 + t->rotation) % 8], t->rgba, t->offset.x + t->extent.x, t->offset.y + t->extent.y, t->layer });
			}
			else {
				vertices.push_back({ coords[(0 + t->rotation) % 8], coords[(1 + t->rotation) % 8], t->rgba, t->offset.x + t->extent.x, t->offset.y , t->layer });
				vertices.push_back({ coords[(4 + t->rotation) % 8], coords[(5 + t->rotation) % 8], t->rgba, t->offset.x, t->offset.y + t->extent.y, t->layer });
			}
		}
	}
	void TilemapAnim::drawMap()
	{
		sceGuEnable(GU_BLEND);
		sceGuEnable(GU_TEXTURE_2D);

		texRef->bindTexture(0, 0, true);

		sceGumDrawArray(GU_SPRITES, GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, vertices.size(), 0, vertices.data());

		sceGuDisable(GU_BLEND);
	}
	void TilemapAnim::loadTileFromJSON(std::string path)
	{
	}
}
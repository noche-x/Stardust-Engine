#pragma once
#include <vector>
#include <Math/Vector.h>
#include <Graphics/TextureUtil.h>
#include <Graphics/RenderTypes.h>
#include <Math/AABB.h>
#include "Light.h"

namespace Stardust::Graphics::Render2D {
	class Sprite2 {
	public:
		Sprite2();
		~Sprite2();
		Sprite2(Texture* tex);
		Sprite2(Texture* tex, glm::vec2 size);
		Sprite2(Texture* tex, glm::vec2 pos, glm::vec2 endPos);

		void position(float x, float y);
		void scale(float x, float y);

		void setAmbientLight(AmbientLight light);
		void addPointLight(PointLight light);
		void calculateLighting();

		void setColor(char r, char g, char b, char a);
		void setColor(int rgba);
		void setLayer(int layer);
		void setTexture(Texture* tex);
		void setPhysics(bool p);
		inline glm::vec2 getPosition() {
			return offset;
		}
		void draw();

		inline Math::AABB2D getAABB() {
			return { offset * 2.0f, extent };
		}

		bool checkPhysics(const Math::AABB2D& aabb);

	protected:
		glm::vec2 offset;
		glm::vec2 extent;
		glm::vec2 scaleFactor;

		Texture* texRef;
		bool physics;

		AmbientLight ambient;
		std::vector<PointLight> pointLights;

		int rgba;
		int layer;
		TextureVertex* vertices;
	};
}
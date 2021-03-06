#include <Graphics/2D/TopDownController.h>
#include <Utilities/Input.h>

namespace Stardust::Graphics::Render2D {

	TopDownController* tp;



	void tdwalkForward(bool, float)
	{
		if(tp->getCharacterSprite()->getFacing() != CHARACTER_FACING_UP)
			tp->getCharacterSprite()->setFacing(CHARACTER_FACING_UP);
		tp->getCharacterSprite()->triggerAnimEvent("walk");
		tp->velocity = { 0.0f, -tp->speed };
	}
	void tdwalkBackward(bool, float)
	{
		if (tp->getCharacterSprite()->getFacing() != CHARACTER_FACING_DOWN)
			tp->getCharacterSprite()->setFacing(CHARACTER_FACING_DOWN);
		tp->getCharacterSprite()->triggerAnimEvent("walk");
		tp->velocity = { 0.0f, tp->speed };
	}
	void tdwalkLeft(bool, float)
	{
		if (tp->getCharacterSprite()->getFacing() != CHARACTER_FACING_LEFT)
			tp->getCharacterSprite()->setFacing(CHARACTER_FACING_LEFT);

		tp->getCharacterSprite()->triggerAnimEvent("walk");
		tp->velocity = { -tp->speed, 0.0f };
	}
	void tdwalkRight(bool, float)
	{
		if (tp->getCharacterSprite()->getFacing() != CHARACTER_FACING_RIGHT)
			tp->getCharacterSprite()->setFacing(CHARACTER_FACING_RIGHT);
		tp->getCharacterSprite()->triggerAnimEvent("walk");
		tp->velocity = { tp->speed, 0.0f };
	}

	void TopDownController::registerHandlers()
	{
		tp = this;
		Utilities::addActionHandler("walkUp", tdwalkForward);
		Utilities::addActionHandler("walkDown", tdwalkBackward);
		Utilities::addActionHandler("walkLeft", tdwalkLeft);
		Utilities::addActionHandler("walkRight", tdwalkRight);
	}

	void TopDownController::update(float dt)
	{
		glm::vec2 newPos = position + velocity * dt;
		doPhysics(newPos);

		setPosition(position);

	}

	TopDownController::TopDownController(CharacterSprite* s, float ss, bool f) : Controller2D(s, f)
	{
		speed = ss;
	}
}
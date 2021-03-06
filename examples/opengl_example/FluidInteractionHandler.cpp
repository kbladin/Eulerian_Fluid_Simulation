#include "FluidInteractionHandler.h"

#include <iostream>

FluidInteractionHandler::FluidInteractionHandler(FluidRendererGL& renderer) :
	_renderer(renderer)
{
	_emitter_radius = 0.05;
}

FluidInteractionHandler::~FluidInteractionHandler()
{

}

void FluidInteractionHandler::windowSizeCallback(int width, int height)
{
  _renderer.camera().setAspectRatio( static_cast<float>(width) / height);
}

void FluidInteractionHandler::step(float dt)
{
    // For now just create on every update
	glm::vec2 st;
    if (_mouse_buttons_pressed.count(MouseButton::MOUSE_BUTTON_1) &&
    	_renderer.intersectsFluidMesh(glm::vec2(_mouse_x, _mouse_y), &st))
	{
		_renderer.fluidDomain().addFluidSource(FluidSource(
		{ st.x - _emitter_radius, st.x + _emitter_radius,
			st.y - _emitter_radius, st.y + _emitter_radius },
		_renderer.fluidDomain().deltaX(),
		_renderer.fluidDomain().deltaY(),
		0.0, 0.0, 0.0, 1));
	}

	// Reset
	_mouse_dx = 0;
	_mouse_dy = 0;
	_scroll_dx = 0;
	_scroll_dy = 0;
}

void FluidInteractionHandler::keyCallback(Key key, KeyAction action)
{
    Controller::keyCallback(key, action);
	if (action == KeyAction::PRESS || action == KeyAction::REPEAT)
	{
		switch (key)
		{
			case Key::KEY_R :
			{
				_renderer.fluidDomain().clearFluidSources();
				_renderer.fluidDomain().resetParticleSet();
                break;
			}
			case Key::KEY_O :
			{
				_renderer.fluidDomain().setPicRatio(_renderer.fluidDomain().picRatio() * 0.8);
                break;
			}
			case Key::KEY_P :
			{
				_renderer.fluidDomain().setPicRatio(_renderer.fluidDomain().picRatio() * 1.2);
                break;
			}
			case Key::KEY_K :
			{
				_emitter_radius -= 0.01;
                break;
			}
			case Key::KEY_L :
			{
				_emitter_radius += 0.01;
                break;
			}
			default: break;
		}
	}
	_emitter_radius = glm::clamp(_emitter_radius, 0.01f, 0.2f);
}

/*
void FluidInteractionHandler::mousePosCallback(double x, double y)
{
	_mouse_x = x;
	_mouse_y = y;
}

void FluidInteractionHandler::mouseButtonCallback(int button, int action, int mods)
{
	glm::vec2 st;
	if (_renderer->intersectsFluidMesh(glm::vec2(_mouse_x, _mouse_y), &st))
	{
		_fluid_domain->addFluidSource(FluidSource(
		{ st.x - _emitter_radius, st.x + _emitter_radius,
			st.y - _emitter_radius, st.y + _emitter_radius },
		_fluid_domain->deltaX(),
		_fluid_domain->deltaY(),
		0.0, 0.0, 0.0, 1));
	}
}

void FluidInteractionHandler::mouseScrollCallback(double dx, double dy)
{

}


void FluidInteractionHandler::windowSizeCallback(int width, int height)
{
	_renderer->setWindowResolution(width, height);
}*/

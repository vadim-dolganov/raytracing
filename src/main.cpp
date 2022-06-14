#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GLWrapper.h"
#include "SceneManager.h"
#include "Surface.h"

void update_scene(scene_container& scene, float delta, float time);

namespace update {
	int jupiter = -1,
		saturn = -1,
		saturn_rings = -1,
		mars = -1,
		box = -1;
}

const glm::quat saturn_pitch = glm::quat(glm::vec3(glm::radians(15.f), 0, 0));


int main()
{
	//GLWrapper glWrapper(1280, 720, false);

	GLWrapper glWrapper(true);

	glWrapper.init_window();

	int windowsWidth = glWrapper.getWidth();
	int windowsHeight = glWrapper.getHeight();

	// fix ray direction issues
	if (windowsWidth % 2 == 1) windowsWidth++;
	if (windowsHeight % 2 == 1) windowsHeight++;

	scene_container scene = {};

	scene.scene = SceneManager::create_scene(windowsWidth, windowsHeight);
	scene.scene.camera_pos = { 0, 0, -5 };
	scene.shadow_ambient = glm::vec3{ 0.1, 0.1, 0.1 };
	scene.ambient_color = glm::vec3{ 0.025, 0.025, 0.025 };

	// lights
    scene.lights_point.push_back(SceneManager::create_light_point({ 3, 4, 0, 0.1 }, { 1, 1, 1 }, 1.5));
	// blue sphere
	scene.spheres.push_back(SceneManager::create_sphere({ 2, 0, 6 }, 1,
		SceneManager::create_material({ 0, 0, 1 }, 50, 0.35)));
	// red sphere
	scene.spheres.push_back(SceneManager::create_sphere({ -1, 0, 6 }, 1,
		SceneManager::create_material({ 1, 0, 0 }, 100, 0.1), true));
	// transparent sphere
	scene.spheres.push_back(SceneManager::create_sphere({ 0.5, 2, 6 }, 1,
		SceneManager::create_material({ 1, 1, 1 }, 200, 0.1, 1.125, { 1, 0, 2 }, 1)));

	// jupiter
	rt_sphere jupiter = SceneManager::create_sphere({}, 5000,
		SceneManager::create_material({}, 0, 0.0f));
	jupiter.textureNum = 1;
	scene.spheres.push_back(jupiter);
	update::jupiter = scene.spheres.size() - 1;

	// saturn
	const int saturnRadius = 4150;
	rt_sphere saturn = SceneManager::create_sphere({}, saturnRadius,
		SceneManager::create_material({}, 0, 0.0f));
	saturn.textureNum = 2;
	saturn.quat_rotation = saturn_pitch;
	scene.spheres.push_back(saturn);
	update::saturn = scene.spheres.size() - 1;

	// mars
	rt_sphere mars = SceneManager::create_sphere({}, 500,
		SceneManager::create_material({}, 0, 0.0f));
	mars.textureNum = 3;
	scene.spheres.push_back(mars);
	update::mars = scene.spheres.size() - 1;

	// sun
	const int sunRadius = 10000;
	glm::vec3 center = { sunRadius * 20, 0, 0 };
	rt_sphere sun = SceneManager::create_sphere(center, sunRadius,
		SceneManager::create_material({}, 10, 0.0f));
	sun.textureNum = 6;
	//scene.spheres.push_back(sun);
	scene.lights_point.push_back(SceneManager::create_light_point({ center.x+ sunRadius*2, center.y, center.z, sunRadius + 10 }, { 1, 1, 1 }, 1.5));

	// ring
	{
		rt_ring ring = SceneManager::create_ring({}, saturnRadius * 1.1166, saturnRadius * 2.35,
			SceneManager::create_material({}, 0, 0));
		ring.textureNum = 4;
		ring.quat_rotation = glm::angleAxis(glm::radians(90.f), glm::vec3(1, 0, 0)) * saturn_pitch;
		scene.rings.push_back(ring);
		update::saturn_rings = scene.rings.size() - 1;
	}

	rt_material coneMaterial = SceneManager::create_material({ 234 / 255.0f, 17 / 255.0f, 82 / 255.0f }, 200, 0.1);
	rt_surface cone = SurfaceFactory::GetEllipticCone(1 / 3.5f, 1 / 3.5f, 1, coneMaterial);
	cone.pos = { -5, 4, 6 };
	cone.quat_rotation = glm::quat(glm::vec3(glm::radians(90.f), 0, 0));
	cone.yMin = -1;
	cone.yMax = 4;
	scene.surfaces.push_back(cone);

	// floor
	scene.boxes.push_back(SceneManager::create_box({ -12, 2.4, 6 }, { 0.2, 4, 12 },
    		SceneManager::create_material({ 1, 0.6, 0 }, 00, 0.0)));
	scene.boxes.push_back(SceneManager::create_box({ 12, 2.4, 6 }, { 0.2, 4, 12 },
    		SceneManager::create_material({ 1, 0.6, 0 }, 00, 0.1)));
	scene.boxes.push_back(SceneManager::create_box({ 0, 2.4, 18 }, { 12, 4, 0.2 },
    		SceneManager::create_material({ 1, 0.6, 0 }, 00, 0.0)));
	scene.boxes.push_back(SceneManager::create_box({ 0, 2.4, -6 }, { 12, 4, 0.2 },
    		SceneManager::create_material({ 1, 0.6, 0 }, 00, 0.0)));
	scene.boxes.push_back(SceneManager::create_box({ 0, -1.2, 6 }, { 12, 0.2, 12 },
    		SceneManager::create_material({ 1, 0.6, 0 }, 00, 0.0)));
    scene.boxes.push_back(SceneManager::create_box({ 0, 6, 6 }, { 12, 0.2, 12 },
    		SceneManager::create_material({ 1, 0.6, 0 }, 00, 0.1)));

	rt_defines defines = scene.get_defines();
	glWrapper.init_shaders(defines);

	std::vector<std::string> faces =
	{
		ASSETS_DIR "/textures/skybox/GalaxyTex_PositiveX.png",
		ASSETS_DIR "/textures/skybox/GalaxyTex_NegativeX.png",
		ASSETS_DIR "/textures/skybox/GalaxyTex_PositiveY.png",
		ASSETS_DIR "/textures/skybox/GalaxyTex_NegativeY.png",
		ASSETS_DIR "/textures/skybox/GalaxyTex_PositiveZ.png",
		ASSETS_DIR "/textures/skybox/GalaxyTex_NegativeZ.png"
	};

	glWrapper.set_skybox(GLWrapper::load_cubemap(faces, false));

	auto jupiterTex = glWrapper.load_texture(1, "planets/8k_jupiter.jpg", "texture_sphere_1");
	auto saturnTex = glWrapper.load_texture(2, "planets/8k_saturn.jpg", "texture_sphere_2");
	auto marsTex = glWrapper.load_texture(3, "planets/2k_mars.jpg", "texture_sphere_3");
	auto ringTex = glWrapper.load_texture(4, "planets/8k_saturn_ring_alpha.png", "texture_ring");
	auto boxTex = glWrapper.load_texture(5, "planets/container.png", "texture_box");
	auto sunTex = glWrapper.load_texture(6, "planets/8k_sun.jpg", "texture_sphere_6");

	SceneManager scene_manager(windowsWidth, windowsHeight, &scene, &glWrapper);
	scene_manager.init();

	float currentTime = static_cast<float>(glfwGetTime());
	float lastFramesPrint = currentTime;
	float framesCount = 0;

	while (!glfwWindowShouldClose(glWrapper.window))
	{
		framesCount++;
		float newTime = static_cast<float>(glfwGetTime());
		float deltaTime = newTime - currentTime;
		currentTime = newTime;

		if (newTime - lastFramesPrint > 1.0f)
		{
			std::cout << "FPS: " << framesCount << std::endl;
			lastFramesPrint = newTime;
			framesCount = 0;
		}

		update_scene(scene, deltaTime, newTime);
		scene_manager.update(deltaTime);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, jupiterTex);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, saturnTex);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, marsTex);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, ringTex);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, sunTex);
		glWrapper.draw();
		glfwSwapBuffers(glWrapper.window);
		glfwPollEvents();
	}

	glWrapper.stop();
	return 0;
}

void update_scene(scene_container& scene, float deltaTime, float time)
{
	if (update::jupiter != -1) {
		rt_sphere* jupiter = &scene.spheres[update::jupiter];
		const float jupiterSpeed = 0.02;
		jupiter->obj.x = cos(time * jupiterSpeed) * 20000;
		jupiter->obj.z = sin(time * jupiterSpeed) * 20000;

		jupiter->quat_rotation *= glm::angleAxis(deltaTime / 15, glm::vec3(0, 1, 0));
	}

	if (update::saturn != -1 && update::saturn_rings != -1) {
		rt_sphere* saturn = &scene.spheres[update::saturn];
		rt_ring* ring = &scene.rings[update::saturn_rings];
		const float speed = 0.0082;
		const float dist = 35000;
		const float offset = 1;

		saturn->obj.x = cos(time * speed + offset) * dist;
		saturn->obj.z = sin(time * speed + offset) * dist;

		glm::vec3 axis = glm::vec3(0, 1, 0) * saturn_pitch;
		saturn->quat_rotation *= glm::angleAxis(deltaTime / 10, axis);

		ring->pos.x = cos(time * speed + offset) * dist;
		ring->pos.z = sin(time * speed + offset) * dist;
	}

	if (update::mars != -1) {
		rt_sphere* mars = &scene.spheres[update::mars];
		const float marsSpeed = 0.05;
		mars->obj.x = cos(time * marsSpeed + 0.5f) * 10000 / 10;
		mars->obj.z = sin(time * marsSpeed + 0.5f) * 10000 / 10;
		mars->obj.y = -cos(time * marsSpeed) * 3000 / 5;
		mars->quat_rotation *= glm::angleAxis(deltaTime / 10, glm::vec3(0, 1, 0));
	}
}
#pragma once

#include <array>
#include "glm/glm.hpp"

constexpr glm::vec3 WORLD_UP{ 0.f, 1.f, 0.f };

#undef near
#undef far

/// <summary>
/// y is up
/// x is right
/// z is forward
/// </summary>
class Camera
{
    /// <summary>
    /// Pre calculating these variables
    glm::vec3		dir;
    glm::vec3		up;
    glm::vec3		right;
    glm::vec3		pos;

    glm::mat4		view, projection;
    glm::vec3		velocity;

    //				in angles
    float			yaw{ -90.f };
    float			pitch{0.f};
    //

    float			fov{ 90.f };
    float			aspect{ 0.f };

    float			near { 0.1f };
    float			far { 1000.f };
    ///

    float			cameraSpeed{10.f};
    float			mouseSensitivity{ 25.f };

    void			UpdateViewMatrix();
    void			UpdateProjectionMatrix();

public:

    Camera();

    /// <summary>
    /// Sets the camera direction by yaw and pitch
    /// </summary>
    /// <param name="yaw"></param>
    /// <param name="pitch"></param>
    void			SetCameraDirection(const float yaw, const float pitch);

    void			SetCameraPosition(const glm::vec3& pos);

    void			ProcessMouseMovement(float x, float y);

    /// <summary>
    /// SetCameraDirection
    /// </summary>
    /// <param name="dir">Normalized direction</param>
    void			SetCameraDirection(const glm::vec3& dir);

    void			SetProjection(float fov, float aspect, float near, float far);

    void			SetFov(const float fov);

    void			SetAspect(const float aspect);

    void			SetNear(const float near);

    void			SetFar(const float far);

    void			AddTranslate(const glm::vec3& trans);

    void			MoveForward(float scale, const float deltaTime);

    void			MoveRight(float scale, const float deltaTime);

    void			MoveUp(float scale, const float deltatime);

    void			SetCameraSpeed(float speed);

    void			SetVelocity(const glm::vec3& velocity);

    void			SetVelocityX(const float x);

    void			SetVelocityY(const float y);

    void			SetVelocityZ(const float z);


    //				To make camera smooth
    void			UpdateCamera(const float deltaTime);

    const glm::vec3 GetPosition() const;
    const glm::mat4 GetViewMatrix() const;
    const glm::mat4 GetProjectionMatrix() const;
    float			GetNear() const;
    float			GetFar()  const;
    float			GetFov()  const;
    glm::vec3		GetDirection() const;
};

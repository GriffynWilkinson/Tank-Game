#ifndef __GameDev2D_Math_h__
#define __GameDev2D_Math_h__

using namespace glm;

namespace GameDev2D
{
    //Local constants
    const float PTM_RATIO = 32.0f; //Pixel to meter ratio
    const float EPSILON = 0.0001f;

    //Forward declaration
    class GameObject;

    class Math
    {
    public:
        static float DegreesToRadians(float degrees);
        static float RadiansToDegrees(float radians);

        static float CalculateDistanceSquared(vec2 pointA, vec2 pointB);

        static float CalculateDistance(vec2 pointA, vec2 pointB);
        static float CalculateDistance(GameObject* a, GameObject* b);

        static float CalculateDistance(float speed, double duration);
        static float CalculateDistance(vec2 displacement);

        static float CalculateSpeed(float distance, double duration);
        static float CalculateSpeed(vec2 velocity);

        static vec2 CalculateLinearVelocity(float speed, float angleInDegrees);
        static vec2 CalculateLinearVelocity(vec2 displacement, double duration);

        static vec2 CalculateDisplacement(float distance, float angleInDegrees);
        static vec2 CalculateDisplacement(vec2 velocity, double duration);

        static vec2 CalculateDirection(float angleInDegrees);

        static float CalculateAngle(float x, float y);
        static float CalculateAngle(vec2 vector);

        static vec2 CalculateOrbit(vec2 origin, float angleInDegrees, float distance);
        static vec2 CalculateOrbit(GameObject* gameObject, float angleInDegrees, float distance);

        static float CalculateSineAmplitude(float value, float wavelength, float amplitude, float frequency);

        static float PixelsToMeters(float pixels);
        static vec2 PixelsToMeters(vec2 pixels);

        static float MetersToPixels(float meters);
        static vec2 MetersToPixels(vec2 meters);

        static float Dot(vec2 A, vec2 B);
        static vec2 Normalize(vec2 Vector);

        static vec2 CalculateClosestPoint(vec2 circle, float radius, vec2 lineStart, vec2 lineEnd);

    };
}

#endif
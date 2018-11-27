#include "Math.h"
#include "../../../Game/GameObject.h"

namespace GameDev2D
{
    float Math::DegreesToRadians(float aDegrees)
    {
        return aDegrees * 0.01745329251f;
    }

    float Math::RadiansToDegrees(float aRadians)
    {
        return aRadians * 57.2957795131f;
    }

    float Math::CalculateDistanceSquared(vec2 aPointA, vec2 aPointB)
    {
        float xSquared = (aPointB.x - aPointA.x) * (aPointB.x - aPointA.x);
        float ySquared = (aPointB.y - aPointA.y) * (aPointB.y - aPointA.y);
        return xSquared + ySquared;
    }

    float Math::CalculateDistance(vec2 aPointA, vec2 aPointB)
    {
        float distance = sqrtf(CalculateDistanceSquared(aPointA, aPointB)); //sqrtf is the built-in function for square-root
        return distance;
    }

    float Math::CalculateDistance(GameObject* aA, GameObject* aB)
    {
        return CalculateDistance(aA->GetPosition(), aB->GetPosition());
    }

    float Math::CalculateDistance(float aSpeed, double aDuration)
    {
        return aSpeed * (float)aDuration;
    }

    float Math::CalculateDistance(vec2 aDisplacement)
    {
        float distance = sqrtf(aDisplacement.x * aDisplacement.x + aDisplacement.y * aDisplacement.y);
        return distance;
    }

    float Math::CalculateSpeed(float aDistance, double aDuration)
    {
        return aDistance / aDuration;
    }

    float Math::CalculateSpeed(vec2 aVelocity)
    {
        float speed = sqrtf(aVelocity.x * aVelocity.x + aVelocity.y * aVelocity.y);
        return speed;
    }

    vec2 Math::CalculateLinearVelocity(float aSpeed, float aAngleInDegrees)
    {
        vec2 direction = Math::CalculateDirection(aAngleInDegrees);
        vec2 linearVelocity = vec2(direction.x * aSpeed, direction.y * aSpeed);
        return linearVelocity;
    }

    vec2 Math::CalculateLinearVelocity(vec2 aDisplacement, double aDuration)
    {
        return vec2(CalculateSpeed(aDisplacement.x, aDuration), CalculateSpeed(aDisplacement.y, aDuration));
    }

    vec2 Math::CalculateDisplacement(float aDistance, float aAngleInDegrees)
    {
        vec2 direction = Math::CalculateDirection(aAngleInDegrees);
        vec2 displacement = vec2(direction.x * aDistance, direction.y * aDistance);
        return displacement;
    }

    vec2 Math::CalculateDisplacement(vec2 aVelocity, double aDuration)
    {
        return vec2(CalculateDistance(aVelocity.x, aDuration), CalculateDistance(aVelocity.y, aDuration));
    }

    vec2 Math::CalculateDirection(float aAngleInDegrees)
    {
        //Convert from degrees to radians
        float angleInRadians = DegreesToRadians(aAngleInDegrees);

        //Calculate the direction vector
        vec2 direction = vec2(cosf(angleInRadians), sinf(angleInRadians));

        //Return the direction vector
        return direction;
    }

    float Math::CalculateAngle(float aX, float aY)
    {
        float angleInRadians = atan2f(aY, aX); //Not a typo
        float angleInDegrees = RadiansToDegrees(angleInRadians);
        return angleInDegrees;
    }

    float Math::CalculateAngle(vec2 aVector)
    {
        return CalculateAngle(aVector.x, aVector.y);
    }

    vec2 Math::CalculateOrbit(vec2 aOrigin, float aAngleInDegrees, float aDistance)
    {
        vec2 offset = Math::CalculateDisplacement(aDistance, aAngleInDegrees);
        return aOrigin + offset;
    }

    vec2 Math::CalculateOrbit(GameObject* aGameObject, float aAngleInDegrees, float aDistance)
    {
        return Math::CalculateOrbit(aGameObject->GetPosition(), aAngleInDegrees, aDistance);
    }

    float Math::CalculateSineAmplitude(float aValue, float aWavelength, float aAmplitude, float aFrequency)
    {
        float wavelength = aWavelength / (M_PI * 2.0f);
        float frequency = aFrequency * (M_PI * 2.0f);
        return sinf(frequency + (aValue / wavelength)) * aAmplitude;
    }


    float Math::PixelsToMeters(float aPixels)
    {
        return aPixels / PTM_RATIO;
    }

    vec2 Math::PixelsToMeters(vec2 aPixels)
    {
        return aPixels / PTM_RATIO;
    }

    float Math::MetersToPixels(float aMeters)
    {
        return aMeters * PTM_RATIO;
    }

    vec2 Math::MetersToPixels(vec2 aMeters)
    {
        return aMeters * PTM_RATIO;
    }

    vec2 Math::Normalize(vec2 aVector)
    {
        float magnitude = Math::CalculateDistance(vec2(0.0f, 0.0f), aVector);
        if (magnitude > EPSILON)
        {
            aVector /= magnitude;
        }
        return aVector;
    }

    float Math::Dot(vec2 aA, vec2 aB)
    {
        return aA.x * aB.x + aA.y * aB.y;
    }

    vec2 Math::CalculateClosestPoint(vec2 aCircle, float aRadius, vec2 aLineStart, vec2 aLineEnd)
    {
        vec2 circleV = aCircle - aLineStart;
        vec2 lineV = aLineEnd - aLineStart;
        vec2 normalV = Math::Normalize(lineV);
        float projection = Math::Dot(circleV, normalV);
        vec2 projectionV = normalV * projection;
        vec2 d = aLineStart + projectionV;
        //vec2 distance = abs(circle - d);

        return (d); //Don't actually return 0, 0 when you implement this method
    }
}
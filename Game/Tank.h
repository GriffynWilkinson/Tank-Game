#ifndef __TANK_H__
#define __TANK_H__

#include "GameObject.h"
#include "../Source/Input/KeyCodes.h"


namespace GameDev2D
{
    //Constants
    const vec2 TANK_STARTING_POSITION[] = { vec2(600.0f, 100.0f), vec2(600.0f, 700.0f) };
    const float TANK_STARTING_ANGLE[] = { 90.0f, 270.0f };
    const float TANK_FORCE_AMOUNT = 60.0f;
    const float TANK_TORQUE_AMOUNT = 35.0f;
    const float TANK_ANGULAR_SPEED = 180.0f;
    const float TURRET_ANGULAR_SPEED = 180.0f;
    //const float TANK_MASS = 200.0f;
    const KeyCode TANK_DEFAULT_INPUT[] = { KEY_CODE_D, KEY_CODE_A, KEY_CODE_W, KEY_CODE_S, KEY_CODE_LEFT, KEY_CODE_RIGHT, KEY_CODE_SPACE};
    const double TANK_FIRE_DELAY = 0.5;

    //Enum to manage the tank input
    enum TankInput
    {
        TankInputLeft = 0,
        TankInputRight,
        TankInputUp,
        TankInputDown,
        TankInputTurretLeft,
        TankInputTurretRight,
        TankInputFireTurret,
        TankInputCount
    };

    enum TankColor
    {
        TankBlue = 0,
        TankGreen,
		TankExploded
    };

    //Forward declarations
    class Sprite;
    namespace Physics
    {
        class Body;
    }

    class Tank : public GameObject
    {
    public:
        Tank(TankColor tankColor);
        ~Tank();

        void Update(double delta);

        float GetWidth();
        float GetHeight();

        void SetLeftInput(KeyCode leftInput);
        void SetRightInput(KeyCode rightInput);
        void SetUpInput(KeyCode upInput);
        void SetDownInput(KeyCode downInput);
        void SetTurretLeftInput(KeyCode leftTurretInput);
        void SetTurretRightInput(KeyCode rightTurretInput);
        void SetFireTurretInput(KeyCode fireTurretInput);

        vec2 GetShellPosition();
        float GetTurretAngle();
		void SetTurrentAngle(float angle);

        bool CanFireShell();
        void ResetFireDelay();

        Sprite* getTurret() { return m_Turret; };

		Physics::Body* GetPhysicsBody();

    private:
        TankColor m_TankColor;
        KeyCode m_Input[TankInputCount];
        Sprite* m_Body;
        Sprite* m_Turret;
        Physics::Body* m_PhysicsBody;
        double m_FireDelay;
    };
}

#endif
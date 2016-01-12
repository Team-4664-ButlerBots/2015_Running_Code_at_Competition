#include "WPILib.h"
//#include "SmartDashboard.h"
//Written by Jude Sauve, programmer for 4664 ButlerBots for 2015; W/ a ton of help from 525 prog mentor :)
class Robot: public SampleRobot
{
	Talon*  	m_leftFrontMotor;
	Talon*  	m_leftRearMotor;
	Talon*  	m_rightFrontMotor;
	Talon*  	m_rightRearMotor;
	RobotDrive*	m_robotDrive;
	Joystick*	m_joystick;
	Joystick* 	m_joystick2;
	Victor* m_lift;
	Victor* m_bite;
	float scaleArm= -0.7, scaleClaw= -1.0, scaleJoy= 0.6;
	float dbdArm=0.2, dbdClaw=0.3, dbdJoy=0.3;

public:
	Robot() {
		m_leftFrontMotor = 	new Talon(2);
		m_leftRearMotor = 	new Talon(3);
		m_rightFrontMotor = new Talon(0);
		m_rightRearMotor = 	new Talon(1);

		m_joystick = 	new Joystick(0);
		m_joystick2= new Joystick(1);
		m_robotDrive = 	new RobotDrive(m_leftFrontMotor, m_leftRearMotor,
				  	  	  	  	      m_rightFrontMotor, m_rightRearMotor);
		m_robotDrive->SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
		m_robotDrive->SetInvertedMotor(RobotDrive::kRearRightMotor, true);
		m_lift= new Victor(4);
		m_bite=new Victor(5);
//		myRobot.SetExpiration(0.1);
//		myRobot.SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
//		myRobot.SetInvertedMotor(RobotDrive::kRearRightMotor, true);
	}

	void Autonomous()
	{
		m_robotDrive->SetSafetyEnabled(false);
		//m_robotDrive->Drive(-0.5, 0.0); 	// drive forwards half speed
		m_robotDrive->MecanumDrive_Cartesian(0, -.45, 0);
		Wait(3.15); 				//    for 2 seconds
		m_robotDrive->MecanumDrive_Cartesian(0.0, 0.0, 0.0); 	// stop robot
	}

	void OperatorControl()
	{
		m_robotDrive->SetSafetyEnabled(true);

		while (IsOperatorControl() && IsEnabled())
		{
			//Drive
			if (m_joystick->GetRawButton(6)) {//Arcade Mode
				m_robotDrive->MecanumDrive_Cartesian(0, scaleJoy*ApplyDeadband(m_joystick->GetY(), dbdJoy),
						scaleJoy*ApplyDeadband(m_joystick->GetX(), dbdJoy));
			} else {//Mec Mode
				m_robotDrive->MecanumDrive_Cartesian(scaleJoy*ApplyDeadband(m_joystick->GetX(), dbdJoy),
						scaleJoy*ApplyDeadband(m_joystick->GetY(), dbdJoy),0);
			}

			//Arm
			m_lift->Set(ApplyDeadband(m_joystick2->GetY(), dbdArm) * scaleArm);

			//Claw
			m_bite->Set(ApplyDeadband(m_joystick2->GetX(), dbdClaw) * scaleClaw);

			Wait(0.005);				// wait for a motor update time
		}
	}

	void Test()//Runs during test mode
	{
	}

	float ApplyDeadband(float rawValue, float deadband) {
		rawValue = Limit(rawValue);
		if(fabs(rawValue) < deadband) return 0.0;
		if(rawValue > 0)			  return (rawValue - deadband) / (1.0 - deadband);
									  return (rawValue + deadband) / (1.0 - deadband);
	}

	float Limit(float value) {
		if(value > 1.0)  return 1.0;
		if(value < -1.0) return -1.0;
						 return value;
	}
};

START_ROBOT_CLASS(Robot);

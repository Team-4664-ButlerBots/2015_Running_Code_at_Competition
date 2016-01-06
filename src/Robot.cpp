#include "WPILib.h"
//#include "SmartDashboard.h"

/**
 * This is a demo program showing the use of the RobotDrive class.
 * The SampleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 *
 * WARNING: While it may look like a good choice to use for your code if you're inexperienced,
 * don't. Unless you know what you are doing, complex code will be much more difficult under
 * this system. Use IterativeRobot or Command-Based instead if you're new.
 */
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
	Encoder* m_armEnc;
	DigitalInput* m_toteH_MinDIO;
	DigitalInput* m_canAnd4ToteH_MaxDIO;
	int encCount;//from minDIO

	//bool toMotion=false;
	Victor* m_bite;
	DigitalInput* m_paradeState;
	DigitalInput* m_chargeState;

//	RobotDrive myRobot; // robot drive system
//	Joystick stick; // only joystick

public:
//	Robot() :
//			myRobot(2, 3, 0, 1),	// these must be initialized in the same order
//			stick(0)		// as they are declared above.
//	{
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
		m_armEnc= new Encoder(6,7);
		m_toteH_MinDIO= new DigitalInput(2);
		m_canAnd4ToteH_MaxDIO= new DigitalInput(3);

		m_bite=new Victor(5);
		m_paradeState= new DigitalInput(1);
		m_chargeState= new DigitalInput(0);

		encCount=0;

		//SmartDashboard.PutNumber("Enc Count from Min", encCount);
		//SmartDashboard.PutBoolean("Min Height?", m_toteH_MinDIO);
		//SmartDashboard.PutBoolean("Max Height?", m_canAnd4ToteH_MaxDIO);

//		myRobot.SetExpiration(0.1);
//		myRobot.SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
//		myRobot.SetInvertedMotor(RobotDrive::kRearRightMotor, true);
	}

	//void RobotInit() override {
//			CameraServer::GetInstance()->SetQuality(50);
			//the camera name (ex "cam0") can be found through the roborio web interface
//			CameraServer::GetInstance()->StartAutomaticCapture("cam0");
	//	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
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
//		myRobot.SetSafetyEnabled(true);

		while (IsOperatorControl() && IsEnabled())
		{
			//Drive
			if (m_joystick->GetRawButton(6)) {
				m_robotDrive->MecanumDrive_Cartesian(0, .6*ApplyDeadband(m_joystick->GetY(), 0.1),
						0.6*ApplyDeadband(m_joystick->GetX(), 0.1));
			} else {
				m_robotDrive->MecanumDrive_Cartesian(0.6*ApplyDeadband(m_joystick->GetX(), 0.1),
						0.6*ApplyDeadband(m_joystick->GetY(), 0.1), 0);
			}

			//Arm
			/*if(m_joystick2->GetRawButton(10) //&& !m_paradeState->Get()
					){//Down Arm
				m_lift->Set(-.8);
			}else if(m_joystick2->GetRawButton(11) //&& !m_chargeState->Get()
					){//Up Arm
				m_lift->Set(.8);
			}else{
				m_lift->Set(0.0);// backup stop
			}*/
			if(m_canAnd4ToteH_MaxDIO->Get()&&(m_joystick2->GetY())>=0){
				m_lift->Set(0);
			}else if(m_toteH_MinDIO->Get()&&(m_joystick2->GetY())<=0){
				m_lift->Set(0);
			}else{
				m_lift->Set(-ApplyDeadband(m_joystick2->GetY(), .2) * .7);
			}

			if(m_joystick->GetRawButton(1))
			m_lift->Set(-ApplyDeadband(m_joystick2->GetY(), .2) * .7);

			if(m_toteH_MinDIO){
				encCount=0;
			}else{
				encCount=m_armEnc->Get();
			}

			//Claw
			/*if(m_joystick2->GetRawButton(2) //&& !m_paradeState->Get()
					){//up
				m_bite->Set(1);
			}else if(m_joystick2->GetRawButton(5) //&& !m_chargeState->Get()
					){//out
				m_bite->Set(-.7);
			}else if(m_joystick2->GetRawButton(4) //&& !m_chargeState->Get()
								){//up
							m_bite->Set(.3);
			}else if(m_joystick2->GetRawButton(3) //&& !m_chargeState->Get()
											){//out
										m_bite->Set(-.3);
			}else{
				m_bite->Set(0.0);//backup stop
			}
			*/
			m_bite->Set(-ApplyDeadband(m_joystick2->GetX(), .3));


			Wait(0.005);				// wait for a motor update time
		}

	}

	/**
	 * Runs during test mode
	 */
	void Test()
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

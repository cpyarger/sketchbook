
#include <stdlib.h>
#include <string.h>
#include "Types.h"
#include "Pellergy.h"


void RunFan(void)
{
	static byte fanCount = 0;

	if (++fanCount >= FanStartupTime)
	{
		fanCount = 0;

		if (startingFan)
		{
			if (BLOWER == BLOWEROFF)
			{
				BLOWER = FanLowSpeed;
			}
			else if (BLOWER == BLOWERHIGH)
			{
				BLOWER = fanSpeedGoal;
				startingFan = FALSE;
			}
			else
			{
				BLOWER++;
			}
		}
	}
}


ISR(TIMER0_COMPA_vect)
{
	static byte interruptCount = 0;

	RunFan();

	if (++interruptCount >= ((1000/2)/LOOP_RATE))
	{	// Only run this 20 times per second (50mS).
		// Timer interrupts every 2mS
		static unsigned int minuteCountdown = 0;
		static int ambientTemp = 0;
		static byte flashCountdown = 1;
		static unsigned int puffCountdown = 0;
		static unsigned int feedCountdown = 0;
		static unsigned int flameTime = 0;
		static byte lowFlameTime = 0;
		static unsigned int cleanoutRound = 0;
		static unsigned int runTimeLimit = 0;
		static unsigned int runTimeTimer = 0;

		interruptCount = 0;

		if (countdown != 0)
			countdown--;

		if (minuteCountdown-- == 0)
		{
			minuteCountdown = ONE_MINUTE;
			if ((furnaceState == STABILIZING) ||  
				(furnaceState == FIRING) ||
				(furnaceState == SHUTTINGDOWN))
			{
				Minutes++;
			}
		}

	    if (--flashCountdown == 0)
	    {
			static byte flashPhase = 0;

			flashPhase ^= 1;
		// flash the green LED when we're idle
		// off when alarm
		// on otherwise
			if (furnaceState == IDLE)
			{
				if (flashPhase)
					set_bit(LED_PORT, GREEN_LED);
				else
					clear_bit(LED_PORT, GREEN_LED);
			}
			else if (furnaceState == ALARM)
			{
				clear_bit(LED_PORT, GREEN_LED);
			}
			else
			{
				set_bit(LED_PORT, GREEN_LED);
			}
		// Flash the red LED when disabled
		// On when ALARM
		// off otherwise
			if (furnaceState == ALARM)
			{
				set_bit(LED_PORT, RED_LED);
			}
			else if (ThermostatIgnore)
			{
				if (!flashPhase)
					set_bit(LED_PORT, RED_LED);
				else
					clear_bit(LED_PORT, RED_LED);
			}
			else
			{
				clear_bit(LED_PORT, RED_LED);
			}
	    	flashCountdown = TWO_SECONDS;
	    }

		switch (furnaceState)
		{
    		case IDLE:
	     		// wait for the thermostat to kick on
				clear_bit(IGNITER_PORT, IGNITER);
				if (AuxUse == IGNITER2)
					clear_bit(AUX_PORT, AUX);	// Turn off the second igniter too
				SetFanSpeed(BLOWEROFF);

		      	if (correctedThermostatValue)
	      		{
					if (averagedLightValue <= LightOffValue)
					{
						cleanoutRound++;
	        			furnaceState = FEEDSTART;
	        			countdown = FeedstartFeedTime + 
									FeedstartPreheat1Time + 
									FeedstartPreheat2Time;
					}
					else
					{	// We detected a flame when it should be absent.
						// Put the flame out before continuing.
						countdown = CoolingLimitTime;
						SetFanSpeed(FanMedSpeed);
        				furnaceState = SHUTTINGDOWN;
					}
	      		}
	      		break;
      
			case FEEDSTART:
				// Turn on igniter for preheat1, then
	    		// feed pellets for startFeedTime, run igniter preheat2 more
				set_bit(IGNITER_PORT, IGNITER);
				if (AuxUse == IGNITER2)
					set_bit(AUX_PORT, AUX);	// Turn on the second igniter too
				SetFanSpeed(BLOWEROFF);

				if (countdown >= (FeedstartPreheat2Time + FeedstartFeedTime))
				{
		      		if (!correctedThermostatValue)
					{	// if we haven't dropped any pellets and the thermostat
						// is cleared, just turn off and go back to idle.
						clear_bit(IGNITER_PORT, IGNITER);
						if (AuxUse == IGNITER2)
							clear_bit(AUX_PORT, AUX);	// Turn off the second igniter too
						clear_bit(FEEDMOTOR_PORT, FEEDMOTOR);
						SetFanSpeed(BLOWEROFF);
						countdown = 0;
        				furnaceState = IDLE;
						break;
					}
				}

				if ((countdown < (FeedstartPreheat2Time + FeedstartFeedTime)) && (countdown > FeedstartPreheat2Time))
				{
					set_bit(FEEDMOTOR_PORT, FEEDMOTOR);
					if (AuxUse == VENT_FAN)
						set_bit(AUX_PORT, AUX);	// Start the vent fan as soon as pellets start dropping
				}
				else
				{
					clear_bit(FEEDMOTOR_PORT, FEEDMOTOR);
				}

	      		if (countdown == 0)
		   		{
	        		furnaceState = IGNITING;
	        		puffCountdown = PuffOnTime + PuffOffTime;
	        		countdown = (7*ONE_MINUTE) - (FeedstartFeedTime + FeedstartPreheat1Time + FeedstartPreheat2Time);
	      		}
	      		break;

			case IGNITING:
	      		// wait for a flame to appear
				set_bit(IGNITER_PORT, IGNITER);
				if (AuxUse == IGNITER2)
					clear_bit(AUX_PORT, AUX);	// Turn off the second igniter too
				clear_bit(FEEDMOTOR_PORT, FEEDMOTOR);
      
				if (countdown == 0)
      			{
					clear_bit(IGNITER_PORT, IGNITER);
					if (AuxUse == IGNITER2)
						clear_bit(AUX_PORT, AUX);	// Turn off the second igniter too
	       			countdown = AlarmSoftBlowTime + AlarmHardBlowTime;
        			furnaceState = ALARM;
					alarmCause = NO_IGNITION;
      			}

      			if (puffCountdown > 0)
        			puffCountdown--;
      			else
        			puffCountdown = PuffOnTime + PuffOffTime;

      			if (puffCountdown >= PuffOffTime)
					SetFanSpeed(FanLowSpeed);
      			else
					SetFanSpeed(BLOWEROFF);

      			if (averagedLightValue < IgnitionLightThreshold)
        			flameTime = 0;
      			else
        			flameTime++;

      			if (flameTime >= TEN_SECONDS)
      			{
        			furnaceState = STABILIZING;
        			countdown = WarmingCompleteTime;
        			feedCountdown = WarmingFeedDelayTime + WarmingFeedOnTime;	// Wait before feeding pellets.
					SetFanSpeed(FanLowSpeed);
					ambientTemp = averagedStackTemp;
      			}
      			break;

    		case STABILIZING:
      			// bring the combustion chamber up to temperature
				clear_bit(IGNITER_PORT, IGNITER);
				if (AuxUse == IGNITER2)
					clear_bit(AUX_PORT, AUX);	// Turn off the second igniter too

      			if (feedCountdown != 0)
        			feedCountdown--;

				// if the stack temp drops as we start up,
				// track the lower temperature.
				if (ambientTemp > averagedStackTemp)
					ambientTemp = averagedStackTemp;

				if (countdown == (WarmingCompleteTime - TWO_MINUTES))
				{	// we've been burning for one minute
					if (averagedStackTemp < (ambientTemp + 20))
					{	// look for a 20C minimum increase in temperature
// disabled the "no heat" failure 
//						countdown = AlarmSoftBlowTime + AlarmHardBlowTime;
//						furnaceState = ALARM;
//						alarmCause = NO_HEAT;
					}
				}

        		if ((feedCountdown <= (WarmingFeedOnTime + WarmingFeedOffTime)) &&
					(feedCountdown > WarmingFeedOffTime) &&
					(averagedLightValue >= BurningLightThreshold))
        		{
					set_bit(FEEDMOTOR_PORT, FEEDMOTOR);
        		}
        		else
        		{
					clear_bit(FEEDMOTOR_PORT, FEEDMOTOR);
        		}

				if (feedCountdown == 0)
          	 		feedCountdown = WarmingFeedOnTime + WarmingFeedOffTime;

				if (averagedLightValue < IgnitionLightThreshold)
				{
	        		puffCountdown = PUFF_INTERVAL + 1;
					countdown = ONE_MINUTE;
					furnaceState = IGNITING;
				}
      			else if (averagedLightValue < BurningLightThreshold)
      			{
					SetFanSpeed(FanLowSpeed);
					if (countdown < THIRTY_SECONDS)
        				countdown = THIRTY_SECONDS;
      			}
      			else
      			{
					SetFanSpeed(FanMedSpeed);
      			}

				if (averagedStackTemp > CalculateTemperature(TemperatureLimit))
				{
	       			countdown = AlarmSoftBlowTime + AlarmHardBlowTime;
        			furnaceState = ALARM;
					alarmCause = OVER_TEMPERATURE;
				}

      			if (countdown == 0)
      			{
					SetFanSpeed(FanHighSpeed);
        			furnaceState = FIRING;
        			countdown = RUN_LOOP_LENGTH;
					runTimeLimit = MaximumBurnTime;
					runTimeTimer = FIFTEEN_MINUTES;
					StartCycles++;
      			}
      			break;

    		case FIRING:
      			// Run the blower on high, feed pellets occasionally
				clear_bit(IGNITER_PORT, IGNITER);
				if (AuxUse == IGNITER2)
					clear_bit(AUX_PORT, AUX);	// Turn off the second igniter too
				SetFanSpeed(FanHighSpeed);

      			if (countdown == 0)
        			countdown = (RunningFeedOnTime + RunningFeedOffTime);

      			if (countdown < RunningFeedOnTime)
						set_bit(FEEDMOTOR_PORT, FEEDMOTOR);
      			else
						clear_bit(FEEDMOTOR_PORT, FEEDMOTOR);

      			if (averagedLightValue < BurningLightThreshold)
      			{
					if (lowFlameTime++ > TEN_SECONDS)
					{	// It's been too low for too long -- ALARM!
	       				countdown = AlarmSoftBlowTime + AlarmHardBlowTime;
        				furnaceState = ALARM;
						alarmCause = FLAME_LOST;
					}
      			}
				else
				{
					lowFlameTime = 0;
				}

				if (averagedStackTemp > CalculateTemperature(TemperatureLimit))
				{
	       			countdown = AlarmSoftBlowTime + AlarmHardBlowTime;
        			furnaceState = ALARM;
					alarmCause = OVER_TEMPERATURE;
				}

				runTimeTimer--;
				if (runTimeTimer == 0)
				{
					runTimeLimit--;
					runTimeTimer = FIFTEEN_MINUTES;
				}
      			if (!correctedThermostatValue || (MaximumBurnTime && (runTimeLimit == 0)))
      			{
					countdown = CoolingLimitTime;
        			furnaceState = SHUTTINGDOWN;
      			}
      			break;

    		case SHUTTINGDOWN:
      			// Wait for flame/embers to be gone
				clear_bit(IGNITER_PORT, IGNITER);
				if (AuxUse == IGNITER2)
					clear_bit(AUX_PORT, AUX);	// Turn off the second igniter too
				clear_bit(FEEDMOTOR_PORT, FEEDMOTOR);
				SetFanSpeed(FanHighSpeed);
      			if (averagedLightValue <= LightOffValue)
      			{
					SetFanSpeed(BLOWERHIGH);
        			furnaceState = PURGING;
        			countdown = CleaningCompleteTime;
      			}
				else if (countdown == 0)
				{	// Flame sense is not going away -- error!
	       			countdown = AlarmSoftBlowTime + AlarmHardBlowTime;
        			furnaceState = ALARM;
					alarmCause = FLAME_SENSED_TOO_LONG;
				}
				else
				{
					flameTime = 0;
				}
      			break;

    		case PURGING:
      			// Run blower for 500 seconds to clean combustion chamber
				{
					static byte sawFlameCount = 0;
					static byte currentPulseCount = 0;

					{
						clear_bit(IGNITER_PORT, IGNITER);
						if (AuxUse == IGNITER2)
							clear_bit(AUX_PORT, AUX);	// Turn off the second igniter too
						clear_bit(FEEDMOTOR_PORT, FEEDMOTOR);
						SetFanSpeed(BLOWERHIGH);
						// Only watch the light value until the first air pulse start time
	      				if ((countdown > (CleaningCompleteTime - CleanoutPulseStart)) && (averagedLightValue > (10 + LightOffValue)))
						{
							if (++sawFlameCount > 5)
	      					{  // if we see glowing, go back one state
								countdown = CoolingLimitTime;
	        					furnaceState = SHUTTINGDOWN;
	      					}
						}
						else
						{
							sawFlameCount = 0;
						}
					}

					if ((CleanoutCycles != 0) &&
						(cleanoutRound >= CleanoutCycles))
					{
						if (currentPulseCount < CleanoutPulseCount)
						{
							unsigned int pulseStartTime;
							unsigned int pulseEndTime;

							pulseStartTime = CleaningCompleteTime - CleanoutPulseStart - (currentPulseCount * (CleanoutPulseOnTime + CleanoutPulseOffTime));
							pulseEndTime = pulseStartTime - CleanoutPulseOnTime;
							if ((countdown <= pulseStartTime) && (countdown > pulseEndTime))
							{
								set_bit(AIRBLAST_PORT, AIRBLAST);
							}
							else if (countdown == pulseEndTime)
							{
								clear_bit(AIRBLAST_PORT, AIRBLAST);
								currentPulseCount++;
							}
						}
					}

	      			if (countdown == 0)
	      			{
						SetFanSpeed(BLOWEROFF);
						if (AuxUse == VENT_FAN)
							clear_bit(AUX_PORT, AUX);
	        			furnaceState = IDLE;
						if ((CleanoutCycles != 0) &&
							(cleanoutRound >= CleanoutCycles))
						{
							cleanoutRound = 0;
	      				}
						currentPulseCount = 0;
					}
				}
      			break;

    		case ALARM:
				clear_bit(IGNITER_PORT, IGNITER);
				if (AuxUse == IGNITER2)
					clear_bit(AUX_PORT, AUX);	// Turn off the second igniter too
				clear_bit(FEEDMOTOR_PORT, FEEDMOTOR);
				clear_bit(AIRBLAST_PORT, AIRBLAST);
				set_bit(ALARMOUT_PORT, ALARMOUT);

      			if (countdown > AlarmHardBlowTime)
      			{
					SetFanSpeed(FanMedSpeed);
      			}
				else if (countdown > 0)
      			{
					SetFanSpeed(FanHighSpeed);
      			}
				else
				{
					SetFanSpeed(BLOWEROFF);
					if (AuxUse == VENT_FAN)
					{
						clear_bit(AUX_PORT, AUX);
					}
					else if (AuxUse == SECONDARY_BURNER)
					{
						if (correctedThermostatValue)
							set_bit(AUX_PORT, AUX);
						else
							clear_bit(AUX_PORT, AUX);
					}
				}
      			break;

    		case FEEDTEST:
      			// feed pellets at the running time, but pause only short times.
				// there is no flame.
				clear_bit(IGNITER_PORT, IGNITER);
				if (AuxUse == IGNITER2)
					clear_bit(AUX_PORT, AUX);	// Turn off the second igniter too

      			if (countdown == 0)
				{
					if (--feedTestLoopCounter == 0)
					{
						clear_bit(FEEDMOTOR_PORT, FEEDMOTOR);
						SetFanSpeed(BLOWEROFF);
  						furnaceState = IDLE;
					}
					else
					{
        				countdown = RunningFeedOnTime + ONE_SECOND;
					}
				}

      			if (countdown < ONE_SECOND)
						clear_bit(FEEDMOTOR_PORT, FEEDMOTOR);
      			else
						set_bit(FEEDMOTOR_PORT, FEEDMOTOR);
      			break;

			case INITIALFEEDTEST:
			case FILLAUGER:
      			if (countdown == 0)
				{
					clear_bit(FEEDMOTOR_PORT, FEEDMOTOR);
					SetFanSpeed(BLOWEROFF);
					furnaceState = IDLE;
				}
				else
				{
					set_bit(FEEDMOTOR_PORT, FEEDMOTOR);
				}
				break;

			case FANTEST:
				break;

			case FANONOFFTEST:
				SetFanSpeed(FanHighSpeed);
				break;

			case BURNINTEST:
#define IGNITER_ON_TIME (7*ONE_MINUTE)
      			if (countdown == 0)
					countdown = FIFTEEN_MINUTES;

				// Igniter runs for 7 minutes, then off
				if (countdown > (FIFTEEN_MINUTES - IGNITER_ON_TIME))
				{
					set_bit(IGNITER_PORT, IGNITER);
					if (AuxUse == IGNITER2)
						set_bit(AUX_PORT, AUX);	// Turn on the second igniter too

				}
				else
				{
					clear_bit(IGNITER_PORT, IGNITER);
					if (AuxUse == IGNITER2)
						clear_bit(AUX_PORT, AUX);	// Turn off the second igniter too
				}

				// Blower runs full speed 2 minutes longer than the igniter
				// Then rotates through fan speeds and off
				if (countdown > (FIFTEEN_MINUTES - IGNITER_ON_TIME - TWO_MINUTES))
				{
					SetFanSpeed(BLOWERHIGH);
				}
				else
				{
					static unsigned int blowerCountdown = 0;

					if (blowerCountdown-- == 0)
					{
						static byte phase = 0;

						blowerCountdown = TEN_SECONDS;
						if (BLOWER == BLOWEROFF)
						{
							phase++;
							if (phase == 1)
							{
		  						SetFanSpeed(FanLowSpeed);
							}
							else if (phase == 2)
							{
		  						SetFanSpeed(FanMedSpeed);
							}
							else if (phase == 3)
							{
	  							SetFanSpeed(FanHighSpeed);
							}
							else
							{
	  							SetFanSpeed(BLOWERHIGH);
								phase = 0;
							}
						}
						else
						{
	  						SetFanSpeed(BLOWEROFF);
						}
					}
				}

				// Feed motor runs 2 seconds of every 4
				if ((countdown % (4*ONE_SECOND)) < TWO_SECONDS)
					set_bit(FEEDMOTOR_PORT, FEEDMOTOR);
				else
					clear_bit(FEEDMOTOR_PORT, FEEDMOTOR);

				// Airblast runs 2 seconds of every 13
				if ((countdown % (13*ONE_SECOND)) < TWO_SECONDS)
					set_bit(AIRBLAST_PORT, AIRBLAST);
				else
					clear_bit(AIRBLAST_PORT, AIRBLAST);

				// Check the heat sensor when hot and cold
				if (countdown == (FIFTEEN_MINUTES - IGNITER_ON_TIME))
				{	// the igniter has been on for 7 minutes
					if (averagedStackTemp < 200)	//(F)
					{
        				furnaceState = ALARM;
						alarmCause = BURNIN_ERROR;
					}
				}

				if (countdown == 1)
				{	// the igniter has been off for 8 minutes
					if (averagedStackTemp > 100)	//(F)
					{
        				furnaceState = ALARM;
						alarmCause = BURNIN_ERROR;
					}
				}

				break;

    		default:
       			countdown = AlarmSoftBlowTime + AlarmHardBlowTime;
      			furnaceState = ALARM;
				alarmCause = BAD_STATE;
      			break;
		}
	}
}

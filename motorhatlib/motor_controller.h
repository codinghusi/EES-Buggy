
template<typename T>
T map(T x, T in_min, T in_max, T out_min, T out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

class MotorController {
    enum class State {
        STOPPED,
        FORWARD,
        ROTATING
    };

    int8_t speed;
    uint8_t startSpeed = 80;
    int16_t startAngle = 0;
    int16_t targetAngle = 0;
    float anglePerSecond = 0;
    float currentAngle = 0;
    State state = State::STOPPED;
    State prevState = State::STOPPED;
    PID pid(1, 1, 1, -1000, 1000);
    Time delta;

    Buggy_Motors motors;

public:

    void setSpeed(uint8_t speed) {
        this->speed = speed;
        motors.setSpeed(map(abs(speed), 0, 100, startSpeed, 100));
    }

    void setTargetAngle(int16_t targetAngle) {
        this->targetAngle = targetAngle;
    }

    void drive() {
        drive(currentAngle, 0);
    }

    void drive(int16_t targetAngle, float anglePerSecond) {
        state = State::FORWARD;
        this->anglePerSecond = anglePerSecond;
        this->startAngle = currentAngle;
        setTargetAngle(target_angle);

        motors.drive();

        pid.resetTime();
        delta.start();
    }

    void rotate(int16_t targetAngle) {
        state = State::ROTATING;
        setTargetAngle(targetAngle);

        if (angleDifference(targetAngle, currentAngle) > 0) {
            motors.rotateRight();
        } else {
            motors.rotateLeft();
        }
    }

    void brake() {
        state = State::STOPPED;
        motors.brake();
    }

    // call periodically
    void setCurrentAngle(float currentAngle) {
        this->currentAngle = currentAngle;
    }

    // call periodically
    void correct() {
        if (state == State::STOPPED) {
            return;
        }

        float diff = abs(angleDifference(currentAngle, targetAngle));
        if (diff < 5) {
            brake();
            break;
        }

        if (state == State::FORWARD) {
            float wipTargetAngle = startAngle + (angleDifference(targetAngle, startAngle) / anglePerSecond) * delta.getElapsedSeconds();
            float correction = pid.calculate() / 1000.;
            if (angleDifference(wipTargetAngle, currentAngle) > 0) {
                motors.forwardTurnRight(correction);
            } else {
                motors.forwardTurnLeft(-correction);
            }
        }
    }
};

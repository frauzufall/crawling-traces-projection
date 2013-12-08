#ifndef DAMPER_H
#define DAMPER_H

    /// 4th order Runge-Kutta integrator

namespace guardacaso {

struct State {
     public:
        double x;          // position
        double v;          // velocity
};

struct Derivative {
     public:
        double dx;          // derivative of position: velocity
        double dv;          // derivative of velocity: acceleration
};


class Damper {

private:

        static double Acceleration(State state, double k, double b) {
             return -k * state.x - b * state.v;
        }

        static Derivative Evaluate(State initial, double dt, Derivative d, double dampK, double dampB) {

             State state;
             state.x = initial.x + d.dx*dt;
             state.v = initial.v + d.dv*dt;

             Derivative output;
             output.dx = state.v;
             output.dv = Acceleration(state, dampK, dampB);
             return output;

        }

public:
        static State Integrate(State inState, double dt, double dampK, double dampB) {

            Derivative start;
            start.dv = 0;
            start.dx = 0;
            Derivative a = Evaluate(inState, 0.0, start, dampK, dampB);
            Derivative b = Evaluate(inState, dt*0.5, a, dampK, dampB);
            Derivative c = Evaluate(inState, dt*0.5, b, dampK, dampB);
            Derivative d = Evaluate(inState, dt, c, dampK, dampB);

            double dxdt = 1.0/6.0 * (a.dx + 2.0*(b.dx + c.dx) + d.dx);
            double dvdt = 1.0/6.0 * (a.dv + 2.0*(b.dv + c.dv) + d.dv);

            State outState;
            outState.x = inState.x + dxdt * dt;
            outState.v = inState.v + dvdt * dt;

            return outState;

        }

protected:
        Damper(){}
        ~Damper(){}

};

}

#endif // DAMPER_H

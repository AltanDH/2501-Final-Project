#ifndef TIMER_H_
#define TIMER_H_

namespace game {

    // A class implementing a simple timer
    class Timer {

        public:
            // Constructor and destructor
            Timer(void);
            ~Timer();

            // Start the timer now: end time given in seconds
            void Start(float end_time); 

            // Check if timer has finished
            bool Finished(void) const;

            // Get time remaining until end
            double TimeLeft(void) const;

        private:
            // Store the starting time and ending time (in seconds)
            double start_time_;  
            double end_time_;

    }; // class Timer

} // namespace game

#endif // TIMER_H_

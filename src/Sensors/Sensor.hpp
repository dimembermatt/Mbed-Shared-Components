/** Interface for sensor subclasses. */


/**
 * The SensorIntf class defines an interface for sensors. It can either be
 * called on demand or put into a thread to execute independently.
 */
class SensorIntf {
    private:
        /* Data to extract. */
        float data;
        
        /* Flags for interrogation. */
        bool sampleEnable;
        bool isReady;

    public:
        /* Custom, non-interruptable sensor interrogation scheme. */
        virtual void sampleSensor(void) = 0;

        /* Generic getter. */
        float getData(void) { return data; }

        void enable(void) { sampleEnable = true; }
        void disable(void) { sampleEnable = false; }
        void setReady(void) { isReady = true; }

        /* Loop function. */
        void loop(void) {
            while (true) {
                if (sampleEnable && isReady) {
                    sampleSensor();
                    isReady = false;
                }
            }
        }
}

/**
 * On demand execution (blocking):
 * 
 * ...
 * sensor.sampleSensor();
 * float data = sensor.getData();
 * ...
 */

/** 
 * Thread execution (nonblocking):
 * 
 * Thread 1:
 *      ...
 *      sensor.enable();
 *      ...
 *      if (condition) { sensor.setReady(); }
 *      ...
 * 
 * Thread 2:
 *      ...
 *      sensor.loop();
 *      ...
 */

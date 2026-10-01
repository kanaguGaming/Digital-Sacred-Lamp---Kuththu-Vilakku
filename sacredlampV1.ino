#include <WiFi.h>
#include <WebServer.h>

// =====================================================
// WIFI
// =====================================================

const char* ssid = "SacredLamp";
const char* password = "12345678";

WebServer server(80);


// =====================================================
// GPIO
// =====================================================

const int LED1 = 14;
const int LED2 = 27;
const int LED3 = 26;
const int LED4 = 25;
const int LED5 = 33;

bool ledState[5] = {
  false,
  false,
  false,
  false,
  false
};


// =====================================================
// HTML
// =====================================================

const char webpage[] PROGMEM = R"rawliteral(

<!DOCTYPE html>

<html>

<head>

<meta name="viewport"
      content="width=device-width,
               initial-scale=1.0,
               maximum-scale=1.0,
               user-scalable=no">

<title></title>

<style>

/* =====================================================
   RESET
   ===================================================== */

* {
    box-sizing: border-box;
    margin: 0;
    padding: 0;
    -webkit-tap-highlight-color: transparent;
}


/* =====================================================
   BODY
   ===================================================== */

html,
body {

    width: 100%;
    height: 100%;

    overflow: hidden;

}


body {

    background:

        radial-gradient(
            circle at 50% 50%,
            #261607 0%,
            #110a03 48%,
            #040302 100%
        );

    display: flex;

    align-items: center;

    justify-content: center;

    touch-action: manipulation;

}


/* =====================================================
   LAMP AREA
   ===================================================== */

.lamp-area {

    position: relative;

    width: min(92vw, 410px);

    height: min(92vh, 470px);

}


/* =====================================================
   LAMP BUTTON
   ===================================================== */

.lamp-button {

    position: absolute;

    width: 126px;

    height: 126px;

    border: none;

    padding: 0;

    background: transparent;

    display: flex;

    align-items: center;

    justify-content: center;

    cursor: pointer;

    outline: none;

    transition:
        transform 0.35s ease,
        opacity 0.35s ease;

}


/* =====================================================
   POSITIONS
   ===================================================== */

#lamp1 {

    top: 0;

    left: 50%;

    transform:
        translateX(-50%);

}


#lamp2 {

    top: 25%;

    right: 0;

}


#lamp3 {

    bottom: 5%;

    right: 3%;

}


#lamp4 {

    bottom: 5%;

    left: 3%;

}


#lamp5 {

    top: 25%;

    left: 0;

}


/* =====================================================
   LAMP SVG
   ===================================================== */

.lamp-svg {

    width: 100px;

    height: 115px;

    overflow: visible;

    /*
       IMPORTANT:
       No blur filter here.
    */

}


/* =====================================================
   METAL
   ===================================================== */

.metal {

    fill: #9a5c17;

    stroke: #e0a743;

    stroke-width: 0.9;

}


.metal-light {

    fill: #d18d2f;

    stroke: #f0bd62;

    stroke-width: 0.7;

}


.metal-dark {

    fill: #4b2809;

    stroke: #9d641e;

    stroke-width: 0.8;

}


.highlight {

    fill: #f4c66a;

    opacity: 0.65;

}


/* =====================================================
   OFF FLAME
   ===================================================== */

.flame {

    fill: #7b430b;

    stroke: #a76a1e;

    stroke-width: 0.7;

}


/* =====================================================
   ON FLAME
   ===================================================== */

.lamp-button.on .flame {

    fill: #ffb52e;

    stroke: #ffe8a0;

}


/* =====================================================
   INNER FLAME
   ===================================================== */

.inner-flame {

    fill: #a96512;

}


.lamp-button.on .inner-flame {

    fill: #fff1b2;

}


/* =====================================================
   LAMP ON
   ===================================================== */

.lamp-button.on {

    z-index: 20;

}


/*
   The lamp itself stays sharp.
   The glow is a separate element.
*/

.lamp-button.on .lamp-glow {

    opacity: 1;

    transform:
        scale(1);

}


/* =====================================================
   LOCALIZED GLOW
   ===================================================== */

.lamp-glow {

    position: absolute;

    width: 110px;

    height: 110px;

    border-radius: 50%;

    left: 50%;

    top: 50%;

    transform:
        translate(-50%, -50%)
        scale(0.72);

    background:

        radial-gradient(
            circle,
            rgba(255, 239, 170, 0.40) 0%,
            rgba(255, 177, 35, 0.24) 25%,
            rgba(255, 116, 0, 0.12) 48%,
            transparent 72%
        );

    opacity: 0;

    pointer-events: none;

    transition:
        opacity 0.45s ease,
        transform 0.65s cubic-bezier(.2,.8,.2,1);

    z-index: -1;

}


/* =====================================================
   ON BREATHING
   ===================================================== */

.lamp-button.on .lamp-glow {

    animation:
        glowPulse 1.8s ease-in-out infinite;

}


@keyframes glowPulse {

    0% {

        opacity: 0.70;

    }

    50% {

        opacity: 1;

    }

    100% {

        opacity: 0.70;

    }

}


/* =====================================================
   FLAME ANIMATION
   ===================================================== */

.lamp-button.on .flame {

    transform-origin:
        50% 100%;

    animation:
        flameMove
        0.65s
        ease-in-out
        infinite
        alternate;

}


@keyframes flameMove {

    0% {

        transform:
            rotate(-3deg)
            scale(0.95);

    }

    50% {

        transform:
            rotate(2deg)
            scale(1.04);

    }

    100% {

        transform:
            rotate(-2deg)
            scale(1.08);

    }

}


/* =====================================================
   INNER FLAME
   ===================================================== */

.lamp-button.on .inner-flame {

    transform-origin:
        50% 100%;

    animation:
        innerFlame
        0.42s
        ease-in-out
        infinite
        alternate;

}


@keyframes innerFlame {

    from {

        transform:
            scale(0.90);

    }

    to {

        transform:
            scale(1.08);

    }

}


/* =====================================================
   IGNITION
   ===================================================== */

.lamp-button.ignite {

    animation:
        igniteLamp
        0.55s
        cubic-bezier(.2,.8,.2,1);

}


@keyframes igniteLamp {

    0% {

        transform:
            scale(0.88);

    }

    40% {

        transform:
            scale(1.12);

    }

    70% {

        transform:
            scale(0.97);

    }

    100% {

        transform:
            scale(1);

    }

}


/* =====================================================
   RIPPLE
   ===================================================== */

.lamp-button::after {

    content: "";

    position: absolute;

    left: 50%;

    top: 50%;

    width: 20px;

    height: 20px;

    border:
        1px solid
        rgba(255, 213, 111, 0.8);

    border-radius: 50%;

    transform:
        translate(-50%, -50%)
        scale(0);

    opacity: 0;

    pointer-events: none;

}


.lamp-button.ripple::after {

    animation:
        ripple 0.75s ease-out;

}


@keyframes ripple {

    0% {

        transform:
            translate(-50%, -50%)
            scale(0);

        opacity: 0.8;

    }

    100% {

        transform:
            translate(-50%, -50%)
            scale(6);

        opacity: 0;

    }

}


/* =====================================================
   PRESS
   ===================================================== */

.lamp-button:active {

    transform:
        scale(0.93);

}


#lamp1:active {

    transform:
        translateX(-50%)
        scale(0.93);

}


/* =====================================================
   RESPONSIVE
   ===================================================== */

@media(max-height: 650px) {

    .lamp-area {

        transform:
            scale(0.82);

    }

}


@media(max-width: 350px) {

    .lamp-area {

        transform:
            scale(0.82);

    }

}


/* =====================================================
   ACCESSIBILITY
   ===================================================== */

@media(prefers-reduced-motion: reduce) {

    .lamp-button.on .flame,
    .lamp-button.on .inner-flame,
    .lamp-button.on .lamp-glow {

        animation: none;

    }

}

</style>

</head>


<body>


<div class="lamp-area">


<!-- ===================================================
     LAMP 1
     =================================================== -->

<button
    id="lamp1"
    class="lamp-button"
    onclick="toggleLamp(1)">

    <div class="lamp-glow"></div>

    <svg
        class="lamp-svg"
        viewBox="0 0 100 120"
        xmlns="http://www.w3.org/2000/svg">

        <!-- Flame -->

        <path
            class="flame"
            d="
                M50 37
                C43 31 45 24 51 14
                C56 24 63 29 58 38
                C56 42 53 43 50 37
                Z
            "/>

        <!-- Inner flame -->

        <path
            class="inner-flame"
            d="
                M50 37
                C47 33 49 28 51 24
                C54 29 55 33 52 37
                Z
            "/>

        <!-- Wick holder -->

        <path
            class="metal-dark"
            d="
                M43 42
                Q50 38 57 42
                L55 47
                Q50 49 45 47
                Z
            "/>

        <!-- Lamp bowl -->

        <path
            class="metal"
            d="
                M20 49
                Q50 63 80 49
                L74 67
                Q50 79 26 67
                Z
            "/>

        <!-- Bowl ornament -->

        <path
            class="metal-light"
            d="
                M27 56
                Q50 67 73 56
                L71 61
                Q50 71 29 61
                Z
            "/>

        <!-- Stem -->

        <path
            class="metal"
            d="
                M43 68
                L57 68
                L58 91
                L42 91
                Z
            "/>

        <!-- Stem ornament -->

        <ellipse
            class="metal-light"
            cx="50"
            cy="76"
            rx="11"
            ry="4"/>

        <ellipse
            class="metal-dark"
            cx="50"
            cy="85"
            rx="9"
            ry="3"/>

        <!-- Base -->

        <path
            class="metal"
            d="
                M35 89
                Q50 94 65 89
                L73 97
                Q50 108 27 97
                Z
            "/>

        <ellipse
            class="highlight"
            cx="50"
            cy="96"
            rx="17"
            ry="3"/>

    </svg>

</button>


<!-- ===================================================
     LAMP 2
     =================================================== -->

<button
    id="lamp2"
    class="lamp-button"
    onclick="toggleLamp(2)">

    <div class="lamp-glow"></div>

    <svg
        class="lamp-svg"
        viewBox="0 0 100 120"
        xmlns="http://www.w3.org/2000/svg">

        <path
            class="flame"
            d="
                M50 37
                C43 31 45 24 51 14
                C56 24 63 29 58 38
                C56 42 53 43 50 37
                Z"/>

        <path
            class="inner-flame"
            d="
                M50 37
                C47 33 49 28 51 24
                C54 29 55 33 52 37
                Z"/>

        <path
            class="metal-dark"
            d="
                M43 42
                Q50 38 57 42
                L55 47
                Q50 49 45 47
                Z"/>

        <path
            class="metal"
            d="
                M20 49
                Q50 63 80 49
                L74 67
                Q50 79 26 67
                Z"/>

        <path
            class="metal-light"
            d="
                M27 56
                Q50 67 73 56
                L71 61
                Q50 71 29 61
                Z"/>

        <path
            class="metal"
            d="
                M43 68
                L57 68
                L58 91
                L42 91
                Z"/>

        <ellipse
            class="metal-light"
            cx="50"
            cy="76"
            rx="11"
            ry="4"/>

        <ellipse
            class="metal-dark"
            cx="50"
            cy="85"
            rx="9"
            ry="3"/>

        <path
            class="metal"
            d="
                M35 89
                Q50 94 65 89
                L73 97
                Q50 108 27 97
                Z"/>

        <ellipse
            class="highlight"
            cx="50"
            cy="96"
            rx="17"
            ry="3"/>

    </svg>

</button>


<!-- ===================================================
     LAMP 3
     =================================================== -->

<button
    id="lamp3"
    class="lamp-button"
    onclick="toggleLamp(3)">

    <div class="lamp-glow"></div>

    <svg
        class="lamp-svg"
        viewBox="0 0 100 120"
        xmlns="http://www.w3.org/2000/svg">

        <path
            class="flame"
            d="
                M50 37
                C43 31 45 24 51 14
                C56 24 63 29 58 38
                C56 42 53 43 50 37
                Z"/>

        <path
            class="inner-flame"
            d="
                M50 37
                C47 33 49 28 51 24
                C54 29 55 33 52 37
                Z"/>

        <path
            class="metal-dark"
            d="
                M43 42
                Q50 38 57 42
                L55 47
                Q50 49 45 47
                Z"/>

        <path
            class="metal"
            d="
                M20 49
                Q50 63 80 49
                L74 67
                Q50 79 26 67
                Z"/>

        <path
            class="metal-light"
            d="
                M27 56
                Q50 67 73 56
                L71 61
                Q50 71 29 61
                Z"/>

        <path
            class="metal"
            d="
                M43 68
                L57 68
                L58 91
                L42 91
                Z"/>

        <ellipse
            class="metal-light"
            cx="50"
            cy="76"
            rx="11"
            ry="4"/>

        <ellipse
            class="metal-dark"
            cx="50"
            cy="85"
            rx="9"
            ry="3"/>

        <path
            class="metal"
            d="
                M35 89
                Q50 94 65 89
                L73 97
                Q50 108 27 97
                Z"/>

        <ellipse
            class="highlight"
            cx="50"
            cy="96"
            rx="17"
            ry="3"/>

    </svg>

</button>


<!-- ===================================================
     LAMP 4
     =================================================== -->

<button
    id="lamp4"
    class="lamp-button"
    onclick="toggleLamp(4)">

    <div class="lamp-glow"></div>

    <svg
        class="lamp-svg"
        viewBox="0 0 100 120"
        xmlns="http://www.w3.org/2000/svg">

        <path
            class="flame"
            d="
                M50 37
                C43 31 45 24 51 14
                C56 24 63 29 58 38
                C56 42 53 43 50 37
                Z"/>

        <path
            class="inner-flame"
            d="
                M50 37
                C47 33 49 28 51 24
                C54 29 55 33 52 37
                Z"/>

        <path
            class="metal-dark"
            d="
                M43 42
                Q50 38 57 42
                L55 47
                Q50 49 45 47
                Z"/>

        <path
            class="metal"
            d="
                M20 49
                Q50 63 80 49
                L74 67
                Q50 79 26 67
                Z"/>

        <path
            class="metal-light"
            d="
                M27 56
                Q50 67 73 56
                L71 61
                Q50 71 29 61
                Z"/>

        <path
            class="metal"
            d="
                M43 68
                L57 68
                L58 91
                L42 91
                Z"/>

        <ellipse
            class="metal-light"
            cx="50"
            cy="76"
            rx="11"
            ry="4"/>

        <ellipse
            class="metal-dark"
            cx="50"
            cy="85"
            rx="9"
            ry="3"/>

        <path
            class="metal"
            d="
                M35 89
                Q50 94 65 89
                L73 97
                Q50 108 27 97
                Z"/>

        <ellipse
            class="highlight"
            cx="50"
            cy="96"
            rx="17"
            ry="3"/>

    </svg>

</button>


<!-- ===================================================
     LAMP 5
     =================================================== -->

<button
    id="lamp5"
    class="lamp-button"
    onclick="toggleLamp(5)">

    <div class="lamp-glow"></div>

    <svg
        class="lamp-svg"
        viewBox="0 0 100 120"
        xmlns="http://www.w3.org/2000/svg">

        <path
            class="flame"
            d="
                M50 37
                C43 31 45 24 51 14
                C56 24 63 29 58 38
                C56 42 53 43 50 37
                Z"/>

        <path
            class="inner-flame"
            d="
                M50 37
                C47 33 49 28 51 24
                C54 29 55 33 52 37
                Z"/>

        <path
            class="metal-dark"
            d="
                M43 42
                Q50 38 57 42
                L55 47
                Q50 49 45 47
                Z"/>

        <path
            class="metal"
            d="
                M20 49
                Q50 63 80 49
                L74 67
                Q50 79 26 67
                Z"/>

        <path
            class="metal-light"
            d="
                M27 56
                Q50 67 73 56
                L71 61
                Q50 71 29 61
                Z"/>

        <path
            class="metal"
            d="
                M43 68
                L57 68
                L58 91
                L42 91
                Z"/>

        <ellipse
            class="metal-light"
            cx="50"
            cy="76"
            rx="11"
            ry="4"/>

        <ellipse
            class="metal-dark"
            cx="50"
            cy="85"
            rx="9"
            ry="3"/>

        <path
            class="metal"
            d="
                M35 89
                Q50 94 65 89
                L73 97
                Q50 108 27 97
                Z"/>

        <ellipse
            class="highlight"
            cx="50"
            cy="96"
            rx="17"
            ry="3"/>

    </svg>

</button>


</div>


<script>

/* =====================================================
   LED TOGGLE
   ===================================================== */

async function toggleLamp(number) {

    const lamp =
        document.getElementById(
            "lamp" + number
        );


    try {

        const response =
            await fetch(
                "/led" + number
            );


        const state =
            await response.text();


        if (state === "ON") {

            /*
             * Restart ignition animation.
             */

            lamp.classList.remove(
                "ignite"
            );

            void lamp.offsetWidth;

            lamp.classList.add(
                "ignite"
            );


            /*
             * Restart ripple.
             */

            lamp.classList.remove(
                "ripple"
            );

            void lamp.offsetWidth;

            lamp.classList.add(
                "ripple"
            );


            /*
             * Illuminate ONLY
             * this lamp.
             */

            lamp.classList.add(
                "on"
            );

        }

        else {

            /*
             * Extinguish ONLY
             * this lamp.
             */

            lamp.classList.remove(
                "on"
            );

        }

    }

    catch(error) {

        /*
         * No visible text.
         * Do nothing if ESP32
         * cannot be reached.
         */

    }

}

</script>


</body>

</html>

)rawliteral";


// =====================================================
// LED HANDLER
// =====================================================

void setLED(int number, int pin) {

    ledState[number - 1] =
        !ledState[number - 1];


    digitalWrite(
        pin,
        ledState[number - 1]
            ? HIGH
            : LOW
    );


    server.send(
        200,
        "text/plain",
        ledState[number - 1]
            ? "ON"
            : "OFF"
    );

}


// =====================================================
// SETUP
// =====================================================

void setup() {

    Serial.begin(115200);


    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    pinMode(LED5, OUTPUT);


    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);


    // Start ESP32 WiFi AP

    WiFi.mode(WIFI_AP);

    WiFi.softAP(
        ssid,
        password
    );


    Serial.println();

    Serial.println(
        "================================"
    );

    Serial.println(
        "       SACRED LAMP"
    );

    Serial.println(
        "================================"
    );

    Serial.print(
        "WiFi: "
    );

    Serial.println(
        ssid
    );

    Serial.print(
        "IP: "
    );

    Serial.println(
        WiFi.softAPIP()
    );


    // =================================================
    // WEB PAGE
    // =================================================

    server.on(
        "/",
        []() {

            server.send_P(
                200,
                "text/html",
                webpage
            );

        }
    );


    // =================================================
    // FIVE LAMPS
    // =================================================

    server.on(
        "/led1",
        []() {

            setLED(1, LED1);

        }
    );


    server.on(
        "/led2",
        []() {

            setLED(2, LED2);

        }
    );


    server.on(
        "/led3",
        []() {

            setLED(3, LED3);

        }
    );


    server.on(
        "/led4",
        []() {

            setLED(4, LED4);

        }
    );


    server.on(
        "/led5",
        []() {

            setLED(5, LED5);

        }
    );


    server.begin();

}


// =====================================================
// LOOP
// =====================================================

void loop() {

    server.handleClient();

}

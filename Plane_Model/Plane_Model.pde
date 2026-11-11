import processing.serial.*;

// ---- settings ----
final String PORT_NAME   = "/dev/cu.usbserial-0001";
final int    BAUD        = 115200;
final int    NUM_SENSORS = 3;
final String[] LABELS = {"Set 1", "Set 2", "Set 3"};
final float[][] SIGN = {
    {1, 1, 1},
    {1, 1, 1},
    {1, 1, 1}
};

float[][] ori = new float[NUM_SENSORS][3];   // [sensor][roll, pitch, yaw] degrees
Serial port;

void settings() {
    size(300 * NUM_SENSORS, 300, P3D);       // 300 px per plane
}

void setup() {
    printArray(Serial.list());
    port = new Serial(this, PORT_NAME, BAUD);
    port.bufferUntil('\n');
}

void draw() {
    background(0);
    lights();
    float cellW = width / (float) NUM_SENSORS;

    for (int i = 0; i < NUM_SENSORS; i++) {
        float roll = ori[i][0], pitch = ori[i][1], yaw = ori[i][2];

        fill(255);
        text(LABELS[i] + "  R " + nf(roll, 0, 1) + "  P " + nf(pitch, 0, 1) + "  Y " + nf(yaw, 0, 1),
             cellW * i + 10, 20);

        pushMatrix();
        translate(cellW * (i + 0.5), height / 2);
        rotateY(radians(SIGN[i][2] * yaw));
        rotateX(radians(SIGN[i][1] * pitch));
        rotateZ(radians(SIGN[i][0] * roll));
        drawPlane();
        popMatrix();
    }
}

void serialEvent(Serial p) {
    String line = p.readStringUntil('\n');
    if (line == null) return;
    // println(line);   // uncomment to see what arrives
    String[] f = split(trim(line), ',');
    if (f.length != 4) return;
    try {
        int id = Integer.parseInt(trim(f[0]));
        if (id < 0 || id >= NUM_SENSORS) return;
        float r  = Float.parseFloat(f[1]);
        float pt = Float.parseFloat(f[2]);
        float y  = Float.parseFloat(f[3]);
        ori[id][0] = r; ori[id][1] = pt; ori[id][2] = y;
    } catch (NumberFormatException e) {
        // partial or garbled line: skip
    }
}

void drawPlane() {
    fill(255, 0, 0, 200);
    box(10, 10, 200);

    fill(0, 0, 255, 200);
    pushMatrix();
    translate(0, 0, -120);
    rotateX(PI/2);
    drawCylinder(0, 20, 20, 8);
    popMatrix();

    fill(0, 255, 0, 200);
    beginShape(TRIANGLES);
    vertex(-100,  2, 30); vertex(0,  2, -80); vertex(100,  2, 30);
    vertex(-100, -2, 30); vertex(0, -2, -80); vertex(100, -2, 30);
    vertex(-2, 0, 98); vertex(-2, -30, 98); vertex(-2, 0, 70);
    vertex( 2, 0, 98); vertex( 2, -30, 98); vertex( 2, 0, 70);
    endShape();
    beginShape(QUADS);
    vertex(-100, 2, 30); vertex(-100, -2, 30); vertex(  0, -2, -80); vertex(  0, 2, -80);
    vertex( 100, 2, 30); vertex( 100, -2, 30); vertex(  0, -2, -80); vertex(  0, 2, -80);
    vertex(-100, 2, 30); vertex(-100, -2, 30); vertex(100, -2,  30); vertex(100, 2,  30);
    vertex(-2,   0, 98); vertex(2,   0, 98); vertex(2, -30, 98); vertex(-2, -30, 98);
    vertex(-2,   0, 98); vertex(2,   0, 98); vertex(2,   0, 70); vertex(-2,   0, 70);
    vertex(-2, -30, 98); vertex(2, -30, 98); vertex(2,   0, 70); vertex(-2,   0, 70);
    endShape();
}

void drawCylinder(float topRadius, float bottomRadius, float tall, int sides) {
    float angle = 0;
    float angleIncrement = TWO_PI / sides;
    beginShape(QUAD_STRIP);
    for (int i = 0; i < sides + 1; ++i) {
        vertex(topRadius*cos(angle), 0, topRadius*sin(angle));
        vertex(bottomRadius*cos(angle), tall, bottomRadius*sin(angle));
        angle += angleIncrement;
    }
    endShape();
    if (bottomRadius != 0) {
        angle = 0;
        beginShape(TRIANGLE_FAN);
        vertex(0, tall, 0);
        for (int i = 0; i < sides + 1; i++) {
            vertex(bottomRadius * cos(angle), tall, bottomRadius * sin(angle));
            angle += angleIncrement;
        }
        endShape();
    }
}

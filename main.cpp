#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.14159265358979323846
#define W 1400
#define H 800
#define MAX_FOUNT 25
#define MAX_STARS 200
#define NUM_LAMPS 8
#define NUM_PALMS 6

/* ================================================================
 *  GLOBAL STATE VARIABLES
 * ================================================================ */
int sceneMode = 0;
int autoModeTimer = 0;
int paused = 0;
float vSpeed = 2.0f;

float cxRoad = 450.0f;

int vCar1State = 0;
float vCar1X = -100.0f;
float vCar1Y = 105.0f;

int vCar2State = 0;
float vCar2X = 350.0f;
float vCar2Y = 450.0f;

float carRedX = 400.0f;
float carBlueX = 100.0f;
float busX = 900.0f;
float truckX = 50.0f;
float bikeX = 700.0f;

float heliX = -200.0f;
float heliY = 680.0f;
int heliDir = 1;
float balloonY = 650.0f;
int ballDir = 1;

float planeX = -300.0f;
float planeY = 730.0f;

float camel1X = 1300.0f;
float camel2X = 300.0f;
float camelAnimTime = 0.0f;

float sailX = 500.0f;
float sail2X = 200.0f;
float cruiseX = 1500.0f;
float cruiseY = 385.0f;
int cruiseState = 0;
int cruiseWaitTimer = 0;
float cloudOff = 0.0f;
float craneAng = 0.0f;
int craneDir = 1;
float turbAng = 0.0f;

float palmSw = 0.0f;
int palmDir = 1;
float sunG = 0.0f;
int trafSt = 0;
int trafTm = 0;

float fntX[MAX_FOUNT], fntY[MAX_FOUNT], fntVY[MAX_FOUNT], fntVX[MAX_FOUNT];
float starX[MAX_STARS], starY[MAX_STARS], starB[MAX_STARS], starSpd[MAX_STARS];

float lampPosX[NUM_LAMPS] = {100, 250, 400, 550, 850, 1000, 1150, 1300};

float palmTreeX[NUM_PALMS] = {160, 350, 580, 900, 1050, 1250};
float palmTreeY[NUM_PALMS] = {155, 160, 365, 365, 155, 160};

/* ================================================================
 *  UTILITY DRAWING FUNCTIONS
 * ================================================================ */

void drawFilledCircle(float cx, float cy, float radius, int segments) {
  int i;
  float angle, x, y;
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(cx, cy);
  for (i = 0; i <= segments; i++) {
    angle = 2.0f * PI * (float)i / (float)segments;
    x = cx + radius * cosf(angle);
    y = cy + radius * sinf(angle);
    glVertex2f(x, y);
  }
  glEnd();
}

void drawCircleOutline(float cx, float cy, float radius, int segments) {
  int i;
  float angle;
  glBegin(GL_LINE_LOOP);
  for (i = 0; i < segments; i++) {
    angle = 2.0f * PI * (float)i / (float)segments;
    glVertex2f(cx + radius * cosf(angle), cy + radius * sinf(angle));
  }
  glEnd();
}

void drawFilledEllipse(float cx, float cy, float rx, float ry, int segments) {
  int i;
  float angle;
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(cx, cy);
  for (i = 0; i <= segments; i++) {
    angle = 2.0f * PI * (float)i / (float)segments;
    glVertex2f(cx + rx * cosf(angle), cy + ry * sinf(angle));
  }
  glEnd();
}

void drawRect(float x1, float y1, float x2, float y2) {
  glBegin(GL_QUADS);
  glVertex2f(x1, y1);
  glVertex2f(x2, y1);
  glVertex2f(x2, y2);
  glVertex2f(x1, y2);
  glEnd();
}

void drawQuad(float x1, float y1, float x2, float y2, float x3, float y3,
              float x4, float y4) {
  glBegin(GL_QUADS);
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glVertex2f(x3, y3);
  glVertex2f(x4, y4);
  glEnd();
}

void drawTri(float x1, float y1, float x2, float y2, float x3, float y3) {
  glBegin(GL_TRIANGLES);
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glVertex2f(x3, y3);
  glEnd();
}

void drawLine(float x1, float y1, float x2, float y2) {
  glBegin(GL_LINES);
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glEnd();
}

void drawThickLine(float x1, float y1, float x2, float y2, float width) {
  glLineWidth(width);
  drawLine(x1, y1, x2, y2);
  glLineWidth(1.0f);
}

void drawArc(float cx, float cy, float r, float start, float end, int seg) {
  int i;
  float step = (end - start) / (float)seg;
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(cx, cy);
  for (i = 0; i <= seg; i++) {
    float a = start + step * (float)i;
    glVertex2f(cx + r * cosf(a), cy + r * sinf(a));
  }
  glEnd();
}

void drawTextS(float x, float y, const char *text) {
  int i;
  glRasterPos2f(x, y);
  for (i = 0; text[i]; i++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
}

void drawTextM(float x, float y, const char *text) {
  int i;
  glRasterPos2f(x, y);
  for (i = 0; text[i]; i++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}

void drawTextL(float x, float y, const char *text) {
  int i;
  glRasterPos2f(x, y);
  for (i = 0; text[i]; i++)
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
}

float dimF(void) {
  return (sceneMode == 2) ? 0.45f : ((sceneMode == 1) ? 0.70f : 1.0f);
}

void drawRectGradientY(float x1, float y1, float x2, float y2, float r1,
                       float g1, float b1, float r2, float g2, float b2) {
  glBegin(GL_QUADS);
  glColor3f(r1, g1, b1);
  glVertex2f(x1, y1);
  glVertex2f(x2, y1);
  glColor3f(r2, g2, b2);
  glVertex2f(x2, y2);
  glVertex2f(x1, y2);
  glEnd();
}

void drawTriGradientY(float x1, float y1, float x2, float y2, float x3,
                      float y3, float r1, float g1, float b1, float r2,
                      float g2, float b2) {
  glBegin(GL_TRIANGLES);
  glColor3f(r1, g1, b1);
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glColor3f(r2, g2, b2);
  glVertex2f(x3, y3);
  glEnd();
}

void drawQuadGradientY(float x1, float y1, float x2, float y2, float x3,
                       float y3, float x4, float y4, float r1, float g1,
                       float b1, float r2, float g2, float b2) {
  glBegin(GL_QUADS);
  glColor3f(r1, g1, b1);
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glColor3f(r2, g2, b2);
  glVertex2f(x3, y3);
  glVertex2f(x4, y4);
  glEnd();
}

/* ================================================================
 *  SKY BACKGROUND - Day/Night gradient
 * ================================================================ */
void drawSky(void) {
  if ((sceneMode == 2)) {

    drawRectGradientY(0, 300, W, H, 0.08f, 0.04f, 0.22f, 0.01f, 0.01f, 0.08f);
  } else if (sceneMode == 1) {

    drawRectGradientY(0, 300, W, H, 1.0f, 0.45f, 0.15f, 0.4f, 0.3f, 0.65f);
  } else {

    drawRectGradientY(0, 300, W, H, 0.6f, 0.85f, 1.0f, 0.2f, 0.5f, 0.95f);
  }
}

/* ================================================================
 *  SUN WITH ANIMATED RAYS / CRESCENT MOON
 * ================================================================ */
void drawSunMoon(void) {
  int i;
  float angle, inR, outR, x1, y1, x2, y2, glowSz;

  if ((sceneMode == 2)) {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.95f, 0.95f, 0.80f, 0.05f);
    drawFilledCircle(700, 660, 100, 50);
    glColor4f(0.95f, 0.95f, 0.80f, 0.08f);
    drawFilledCircle(700, 660, 80, 50);
    glDisable(GL_BLEND);

    glColor3f(0.95f, 0.95f, 0.80f);
    drawFilledCircle(700, 660, 50, 50);

    glColor3f(0.88f, 0.88f, 0.74f);
    drawFilledCircle(685, 655, 4, 10);
    drawFilledCircle(695, 672, 3, 10);
    drawFilledCircle(678, 668, 2, 8);
  } else {

    float sunY = (sceneMode == 1) ? 480.0f : 620.0f;

    glowSz = 100.0f + 8.0f * sinf(sunG * 1.5f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 0.95f, 0.50f, 0.12f);
    drawFilledCircle(700, sunY, glowSz + 20, 50);
    glColor4f(1.0f, 0.95f, 0.50f, 0.20f);
    drawFilledCircle(700, sunY, glowSz, 50);
    glDisable(GL_BLEND);

    glColor3f(1.0f, 0.92f, 0.35f);
    drawFilledCircle(700, sunY, 82, 50);
    glColor3f(1.0f, 0.90f, 0.20f);
    drawFilledCircle(700, sunY, 70, 50);
    glColor3f(1.0f, 0.95f, 0.60f);
    drawFilledCircle(700, sunY + 5.0f, 35, 40);

    glColor3f(1.0f, 0.85f, 0.10f);
    glLineWidth(2.5f);
    for (i = 0; i < 24; i++) {
      angle = 2.0f * PI * (float)i / 24.0f + sunG * 0.4f;
      inR = 80.0f;
      outR = 105.0f + 6.0f * sinf(sunG * 2.0f + (float)i * 0.5f);
      x1 = 700 + inR * cosf(angle);
      y1 = sunY + inR * sinf(angle);
      x2 = 700 + outR * cosf(angle);
      y2 = sunY + outR * sinf(angle);
      drawLine(x1, y1, x2, y2);
    }
    glLineWidth(1.0f);
  }
}

/* ================================================================
 *  TWINKLING STARS (Night only) with Milky Way
 * ================================================================ */
void drawStars(void) {
  int i;
  float brightness, mx, my, mb;

  if (!(sceneMode == 2))
    return;

  for (i = 0; i < MAX_STARS; i++) {
    brightness = starB[i] * (0.4f + 0.6f * sinf(sunG * starSpd[i] + (float)i));
    if (brightness < 0.0f)
      brightness = 0.0f;
    glColor3f(brightness, brightness, brightness * 0.92f);
    glPointSize(1.0f + (float)(i % 3));
    glBegin(GL_POINTS);
    glVertex2f(starX[i], starY[i]);
    glEnd();
  }

  for (i = 0; i < MAX_STARS; i += 15) {
    brightness = 0.75f + 0.25f * sinf(sunG * 4.0f + (float)i);
    glColor3f(brightness, brightness, brightness * 0.9f);
    glPointSize(3.5f);
    glBegin(GL_POINTS);
    glVertex2f(starX[i], starY[i]);
    glEnd();
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  for (i = 0; i < 70; i++) {
    mx = (float)(i * 20);
    my = 620.0f + 50.0f * sinf(mx * 0.003f);
    mb = 0.06f + 0.03f * sinf((float)i * 0.3f);
    glColor4f(0.7f, 0.7f, 0.9f, mb);
    drawFilledCircle(mx, my, 12 + (i % 5) * 3, 8);
  }
  glDisable(GL_BLEND);
  glPointSize(1.0f);
}

/* ================================================================
 *  ANIMATED CLOUDS (5 clouds with different speeds)
 * ================================================================ */
void drawClouds(void) {
  int c;
  float speeds[] = {1.0f, 0.7f, 1.3f, 0.5f, 0.9f};
  float startXs[] = {200, 650, 1050, 350, 800};
  float ys[] = {700, 685, 725, 745, 695};
  float sizes[] = {1.0f, 0.85f, 0.9f, 0.6f, 0.75f};
  float cx, s, cy;

  if ((sceneMode == 2))
    glColor3f(0.18f, 0.18f, 0.32f);
  else
    glColor3f(1.0f, 1.0f, 1.0f);

  for (c = 0; c < 5; c++) {
    cx = fmodf(startXs[c] + cloudOff * speeds[c], W + 350) - 175;
    s = sizes[c];
    cy = ys[c];
    drawFilledEllipse(cx, cy, 60 * s, 25 * s, 25);
    drawFilledEllipse(cx + 30 * s, cy + 10 * s, 42 * s, 20 * s, 25);
    drawFilledEllipse(cx - 28 * s, cy - 5 * s, 38 * s, 18 * s, 25);
    drawFilledEllipse(cx + 10 * s, cy + 15 * s, 32 * s, 14 * s, 20);
    drawFilledEllipse(cx - 15 * s, cy + 8 * s, 28 * s, 12 * s, 20);
  }
}


/* ================================================================
 *  DESERT GROUND with sand dunes and rocks
 * ================================================================ */
void drawGround(void) {
  int i;
  float d = dimF();

  drawRectGradientY(0, 140, W, 370, 0.80f * d, 0.65f * d, 0.30f * d, 0.95f * d,
                    0.85f * d, 0.50f * d);

  glColor3f(0.96f * d, 0.86f * d, 0.52f * d);
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(180, 210);
  for (i = 0; i <= 30; i++)
    glVertex2f(180 + 130 * cosf(PI * i / 30), 210 + 30 * sinf(PI * i / 30));
  glEnd();

  glColor3f(0.95f * d, 0.85f * d, 0.50f * d);
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(1100, 190);
  for (i = 0; i <= 25; i++)
    glVertex2f(1100 + 110 * cosf(PI * i / 25), 190 + 22 * sinf(PI * i / 25));
  glEnd();

  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(650, 185);
  for (i = 0; i <= 20; i++)
    glVertex2f(650 + 80 * cosf(PI * i / 20), 185 + 18 * sinf(PI * i / 20));
  glEnd();

  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(1300, 175);
  for (i = 0; i <= 20; i++)
    glVertex2f(1300 + 70 * cosf(PI * i / 20), 175 + 15 * sinf(PI * i / 20));
  glEnd();

  glColor3f(0.65f * d, 0.55f * d, 0.35f * d);
  drawFilledCircle(320, 155, 5, 8);
  drawFilledCircle(540, 150, 4, 8);
  drawFilledCircle(920, 158, 6, 8);
  drawFilledCircle(1050, 148, 3, 8);
  drawFilledEllipse(750, 152, 7, 4, 10);
  drawFilledEllipse(280, 148, 5, 3, 8);
  drawFilledEllipse(460, 153, 6, 3, 8);

  glColor3f(0.55f * d, 0.45f * d, 0.30f * d);
  drawFilledEllipse(380, 160, 12, 7, 12);
  drawFilledEllipse(1000, 155, 10, 6, 10);

  glColor3f(0.50f * d, 0.40f * d, 0.25f * d);
  drawFilledEllipse(380, 163, 8, 4, 10);
  drawFilledEllipse(1000, 158, 7, 3, 8);
}

/* ================================================================
 *  NILE RIVER with animated waves and moonlight reflection
 * ================================================================ */
void drawRiver(void) {
  int i, j;
  float waveX, x, y;

  if ((sceneMode == 2))
    drawRectGradientY(0, 370, W, 435, 0.03f, 0.08f, 0.30f, 0.08f, 0.15f, 0.45f);
  else if (sceneMode == 1)
    drawRectGradientY(0, 370, W, 435, 0.8f, 0.4f, 0.3f, 0.4f, 0.3f, 0.6f);
  else
    drawRectGradientY(0, 370, W, 435, 0.15f, 0.45f, 0.75f, 0.30f, 0.65f, 0.95f);

  if ((sceneMode == 2))
    glColor3f(0.08f, 0.14f, 0.40f);
  else if (sceneMode == 1)
    glColor3f(0.85f, 0.45f, 0.35f);
  else
    glColor3f(0.35f, 0.70f, 0.98f);
  drawRect(0, 430, W, 435);

  if ((sceneMode == 2))
    glColor3f(0.10f, 0.18f, 0.48f);
  else if (sceneMode == 1)
    glColor3f(0.9f, 0.5f, 0.4f);
  else
    glColor3f(0.35f, 0.68f, 0.96f);
  for (i = 0; i < W; i += 40) {
    waveX = (float)i + fmodf(cloudOff * 2.0f, 40.0f);
    glBegin(GL_LINE_STRIP);
    for (j = 0; j < 40; j++) {
      x = waveX + (float)j;
      y = 400.0f + 3.5f * sinf(x * 0.1f + cloudOff * 0.05f);
      glVertex2f(x, y);
    }
    glEnd();
  }

  for (i = 0; i < W; i += 55) {
    waveX = (float)i + fmodf(cloudOff * 1.5f, 55.0f) + 20;
    glBegin(GL_LINE_STRIP);
    for (j = 0; j < 40; j++) {
      x = waveX + (float)j;
      y = 390.0f + 2.5f * sinf(x * 0.12f + cloudOff * 0.07f + 1.0f);
      glVertex2f(x, y);
    }
    glEnd();
  }

  for (i = 0; i < W; i += 65) {
    waveX = (float)i + fmodf(cloudOff * 1.8f, 65.0f) + 10;
    glBegin(GL_LINE_STRIP);
    for (j = 0; j < 35; j++) {
      x = waveX + (float)j;
      y = 410.0f + 2.0f * sinf(x * 0.08f + cloudOff * 0.04f + 2.0f);
      glVertex2f(x, y);
    }
    glEnd();
  }

  if ((sceneMode == 2)) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.95f, 0.95f, 0.80f, 0.06f);
    drawFilledEllipse(700, 400, 50, 10, 25);
    glColor4f(0.95f, 0.95f, 0.80f, 0.04f);
    drawFilledEllipse(700, 400, 90, 15, 25);
    glDisable(GL_BLEND);
  }
}

/* ================================================================
 *  MAIN ROAD with lanes, markings, and zebra crossing
 * ================================================================ */
void drawRoad(void) {
  int i;

  if ((sceneMode == 2))
    glColor3f(0.20f, 0.20f, 0.22f);
  else
    glColor3f(0.35f, 0.35f, 0.38f);
  drawRect(0, 50, W, 140);

  glColor3f(1.0f, 1.0f, 1.0f);
  drawRect(0, 138, W, 140);
  drawRect(0, 50, W, 52);

  for (i = 0; i < W; i += 45) {
    glColor3f(1.0f, 1.0f, 1.0f);
    drawRect(i, 93, i + 22, 97);
  }

  for (i = 0; i < 10; i++) {
    glColor3f(1.0f, 1.0f, 1.0f);
    drawRect(650 + i * 11, 52, 650 + i * 11 + 5, 138);
  }

  if ((sceneMode == 2))
    glColor3f(0.30f, 0.28f, 0.25f);
  else
    glColor3f(0.70f, 0.65f, 0.55f);
  drawRect(0, 140, W, 155);

  if ((sceneMode == 2))
    glColor3f(0.35f, 0.32f, 0.28f);
  else
    glColor3f(0.75f, 0.70f, 0.60f);
  drawRect(0, 140, W, 145);

  if ((sceneMode == 2))
    glColor3f(0.15f, 0.15f, 0.15f);
  else
    glColor3f(0.28f, 0.28f, 0.30f);
  drawRect(0, 0, W, 50);
}

/* ================================================================
 *  PYRAMIDS (3 regular + 1 stepped)
 * ================================================================ */
void drawPyramid(float x, float y, float w, float h) {
  float d = dimF();

  drawTriGradientY(x - w / 2, y, x, y, x, y + h, 0.85f * d, 0.72f * d,
                   0.40f * d, 0.95f * d, 0.85f * d, 0.50f * d);

  drawTriGradientY(x, y, x + w / 2, y, x, y + h, 0.55f * d, 0.45f * d,
                   0.25f * d, 0.75f * d, 0.65f * d, 0.40f * d);

  glColor3f(0.50f * d, 0.40f * d, 0.20f * d);
  drawLine(x, y, x, y + h);

  // Simple stone block texture
  glLineWidth(0.5f);
  int numLayers = 16;
  for (int i = 1; i < numLayers; i++) {
    float frac = (float)i / (float)numLayers;
    float ly = y + h * frac;
    float lw = w * (1.0f - frac) / 2.0f;
    float lwPrev = w * (1.0f - (float)(i - 1) / (float)numLayers) / 2.0f;

    // Horizontal layer lines
    glColor3f(0.80f * d, 0.68f * d, 0.35f * d); // Highlight side
    drawLine(x - lw, ly, x, ly);
    glColor3f(0.45f * d, 0.35f * d, 0.18f * d); // Shadow side
    drawLine(x, ly, x + lw, ly);

    // Vertical block cuts
    int cuts = (int)((1.0f - frac) * 8);
    if (cuts > 0) {
      float stepX = lwPrev / (cuts + 1.5f);
      for (int j = 1; j <= cuts; j++) {
        float stagger = (i % 2 == 0) ? stepX * 0.5f : 0.0f;
        float cutX = stepX * j + stagger;
        if (cutX < lw) {
          // Highlight side cuts
          glColor3f(0.82f * d, 0.70f * d, 0.38f * d);
          drawLine(x - cutX, ly, x - cutX, ly - (h / numLayers));
          // Shadow side cuts
          glColor3f(0.40f * d, 0.30f * d, 0.15f * d);
          drawLine(x + cutX, ly, x + cutX, ly - (h / numLayers));
        }
      }
    }
  }
  glLineWidth(1.0f);
}

void drawSteppedPyramid(float x, float y, float bw, float bh) {
  int i, steps = 6;
  float d = dimF();
  float w, h, yy;

  for (i = 0; i < steps; i++) {
    w = bw - i * (bw / steps);
    h = bh / steps;
    yy = y + i * h;

    glColor3f((0.85f - 0.04f * i) * d, (0.70f - 0.03f * i) * d, 0.35f * d);
    drawRect(x - w / 2, yy, x + w / 2, yy + h);

    glColor3f((0.70f - 0.04f * i) * d, (0.58f - 0.03f * i) * d, 0.28f * d);
    drawRect(x, yy, x + w / 2, yy + h);

    glColor3f((0.60f - 0.03f * i) * d, (0.50f - 0.02f * i) * d, 0.25f * d);
    drawLine(x - w / 2, yy + h, x + w / 2, yy + h);
  }
}

/* ================================================================
 *  EGYPTIAN OBELISK with hieroglyphic details
 * ================================================================ */
void drawObelisk(float x, float y) {
  float d = dimF();
  int i;

  drawQuadGradientY(x - 8, y, x + 8, y, x + 6, y + 95, x - 6, y + 95, 0.75f * d,
                    0.65f * d, 0.40f * d, 0.85f * d, 0.75f * d, 0.50f * d);

  drawTriGradientY(x - 6, y + 95, x + 6, y + 95, x, y + 115, 0.85f * d,
                   0.75f * d, 0.45f * d, 0.90f * d, 0.85f * d, 0.55f * d);

  glColor3f(0.95f * d, 0.85f * d, 0.30f * d);
  drawTri(x - 3, y + 105, x + 3, y + 105, x, y + 115);

  drawQuadGradientY(x, y, x + 8, y, x + 6, y + 95, x, y + 95, 0.60f * d,
                    0.52f * d, 0.32f * d, 0.70f * d, 0.62f * d, 0.42f * d);

  glColor3f(0.65f * d, 0.55f * d, 0.35f * d);
  for (i = 0; i < 8; i++) {
    float ly = y + 10 + i * 10;
    drawLine(x - 5, ly, x + 5, ly);
  }

  drawLine(x, y + 5, x, y + 90);

  for (i = 0; i < 6; i++) {
    float ly = y + 15 + i * 13;
    drawFilledCircle(x - 3, ly, 1, 6);
    drawFilledCircle(x + 3, ly + 6, 1, 6);
  }
}

/* ================================================================
 *  ANCIENT EGYPTIAN TEMPLE with columns and torches
 * ================================================================ */
void drawTemple(float x, float y) {
  float d = dimF();
  int i;
  float cx;

  drawRectGradientY(x - 5, y, x + 165, y + 8, 0.60f * d, 0.45f * d, 0.25f * d,
                    0.65f * d, 0.50f * d, 0.30f * d);

  glColor3f(0.62f * d, 0.48f * d, 0.28f * d);
  drawRect(x, y + 8, x + 160, y + 15);

  for (i = 0; i < 6; i++) {
    cx = x + 14 + i * 26;

    drawRectGradientY(cx - 5, y + 15, cx + 5, y + 68, 0.55f * d, 0.42f * d,
                      0.24f * d, 0.70f * d, 0.55f * d, 0.32f * d);

    glColor3f(0.65f * d, 0.50f * d, 0.30f * d);
    drawRect(cx - 7, y + 15, cx + 7, y + 19);

    glColor3f(0.68f * d, 0.52f * d, 0.32f * d);
    drawRect(cx - 7, y + 65, cx + 7, y + 70);

    glColor3f(0.50f * d, 0.38f * d, 0.20f * d);
    drawLine(cx - 2, y + 19, cx - 2, y + 65);
    drawLine(cx + 2, y + 19, cx + 2, y + 65);
  }

  glColor3f(0.65f * d, 0.50f * d, 0.30f * d);
  drawRect(x - 5, y + 70, x + 165, y + 80);

  glColor3f(0.55f * d, 0.42f * d, 0.25f * d);
  drawRect(x - 8, y + 80, x + 168, y + 84);

  glColor3f(0.68f * d, 0.52f * d, 0.32f * d);
  drawTri(x + 30, y + 84, x + 130, y + 84, x + 80, y + 105);

  glColor3f(0.60f * d, 0.45f * d, 0.25f * d);
  drawTri(x + 50, y + 84, x + 110, y + 84, x + 80, y + 98);

  glColor3f(0.45f * d, 0.30f * d, 0.15f * d);
  drawFilledEllipse(x + 80, y + 90, 8, 4, 12);
  glColor3f(0.20f * d, 0.15f * d, 0.10f * d);
  drawFilledCircle(x + 80, y + 90, 2.5f, 8);

  if ((sceneMode == 2)) {

    glColor3f(0.55f, 0.35f, 0.10f);
    drawRect(x + 5, y + 15, x + 8, y + 35);
    glColor3f(1.0f, 0.7f, 0.2f);
    drawFilledCircle(x + 6.5f, y + 37, 5, 10);
    glColor3f(1.0f, 0.9f, 0.4f);
    drawFilledCircle(x + 6.5f, y + 38, 3, 8);

    glColor3f(0.55f, 0.35f, 0.10f);
    drawRect(x + 152, y + 15, x + 155, y + 35);
    glColor3f(1.0f, 0.7f, 0.2f);
    drawFilledCircle(x + 153.5f, y + 37, 5, 10);
    glColor3f(1.0f, 0.9f, 0.4f);
    drawFilledCircle(x + 153.5f, y + 38, 3, 8);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 0.7f, 0.2f, 0.06f);
    drawFilledCircle(x + 6.5f, y + 37, 25, 20);
    drawFilledCircle(x + 153.5f, y + 37, 25, 20);
    glDisable(GL_BLEND);
  }
}

/* ================================================================
 *  CAMEL DRAWING
 * ================================================================ */
void drawCamel(float x, float y, float scale, int dir, float time) {
  float d = dimF();
  glPushMatrix();
  glTranslatef(x, y, 0);
  glScalef(scale * dir, scale, 1.0f);

  glColor3f(0.70f * d, 0.55f * d, 0.35f * d);

  float l1 = sinf(time) * 4.0f;
  float l2 = sinf(time + PI) * 4.0f;

  drawRect(-6 + l1, 0, -3 + l1, 15);
  drawRect(-5 + l2, 0, -2 + l2, 15);
  drawRect(8 + l2, 0, 11 + l2, 15);
  drawRect(9 + l1, 0, 12 + l1, 15);

  drawFilledEllipse(2, 20, 14, 8, 12);
  drawFilledCircle(-2, 26, 6, 12);
  drawFilledCircle(6, 26, 6, 12);

  drawQuad(12, 18, 15, 17, 20, 30, 17, 31);

  drawFilledEllipse(20, 32, 5, 3, 10);
  drawRect(23, 31, 26, 33);

  drawLine(-12, 20, -14, 12);

  glPopMatrix();
}

/* ================================================================
 *  RUINED EGYPTIAN TEMPLES (Near electric poles)
 * ================================================================ */
void drawRuinedTemples(float bx, float by) {
  float d = dimF() * 0.90f;
  float x, y, cx;
  int i, j;

  x = bx;
  y = by;
  float w1 = 140.0f;
  float h1 = 70.0f;

  drawRectGradientY(x, y, x + w1, y + h1, 0.85f * d, 0.70f * d, 0.40f * d,
                    1.00f * d, 0.85f * d, 0.55f * d);

  glColor3f(0.35f * d, 0.20f * d, 0.12f * d);
  drawRect(x + 20, y + 20, x + w1 - 20, y + h1);

  drawRectGradientY(x, y, x + 35, y + h1, 0.90f * d, 0.75f * d, 0.45f * d,
                    1.00f * d, 0.90f * d, 0.60f * d);

  glColor3f(0.30f * d, 0.18f * d, 0.12f * d);
  drawRect(x + 10, y, x + 25, y + 25);
  glColor3f(0.80f * d, 0.65f * d, 0.40f * d);
  drawRect(x + 8, y + 25, x + 27, y + 30);

  for (i = 0; i < 4; i++) {
    cx = x + 50 + i * 25;
    drawRectGradientY(cx - 4, y, cx + 4, y + h1, 0.85f * d, 0.70f * d,
                      0.42f * d, 1.00f * d, 0.90f * d, 0.65f * d);

    glColor3f(0.70f * d, 0.55f * d, 0.35f * d);
    for (j = 1; j < 5; j++) {
      float secY = y + (h1 / 5.0f) * j;
      drawLine(cx - 4, secY, cx + 4, secY);
    }

    glColor3f(0.95f * d, 0.80f * d, 0.55f * d);
    drawRect(cx - 6, y, cx + 6, y + 4);
    drawRect(cx - 6, y + h1 - 5, cx + 6, y + h1);

    glColor3f(0.60f * d, 0.45f * d, 0.25f * d);
    drawRect(cx - 4, y + h1 - 7, cx + 4, y + h1 - 5);
  }

  glColor3f(0.96f * d, 0.85f * d, 0.60f * d);
  drawRect(x - 5, y + h1, x + w1 + 5, y + h1 + 10);
  glColor3f(0.50f * d, 0.35f * d, 0.20f * d);
  drawQuad(x + 20, y + h1, x + 25, y + h1 + 10, x + w1 - 20, y + h1 + 10,
           x + w1 - 25, y + h1);

  x = bx + 145.0f;
  float w2 = 130.0f;
  float h2 = 85.0f;

  for (i = 0; i < 6; i++) {
    float stepX = x + i * 15;
    float stepY = y + h2 - i * 12;
    if (i > 4)
      stepY = y + h2 - 4 * 12;
    drawRectGradientY(stepX, y, x + w2, stepY, 0.82f * d, 0.65f * d, 0.42f * d,
                      0.98f * d, 0.82f * d, 0.55f * d);

    glColor3f(0.70f * d, 0.55f * d, 0.35f * d);
    drawLine(stepX, stepY, stepX + 15, stepY);
    drawLine(stepX, y, stepX, stepY);
  }

  for (i = 0; i < 5; i++) {
    cx = x + 40 + i * 20;
    drawRectGradientY(cx - 3, y, cx + 3, y + h2 - 10, 0.85f * d, 0.70f * d,
                      0.42f * d, 1.00f * d, 0.88f * d, 0.60f * d);

    glColor3f(0.65f * d, 0.50f * d, 0.30f * d);
    for (j = 1; j < 6; j++) {
      float secY = y + ((h2 - 10) / 6.0f) * j;
      drawLine(cx - 3, secY, cx + 3, secY);
    }

    glColor3f(0.95f * d, 0.80f * d, 0.55f * d);
    drawRect(cx - 5, y, cx + 5, y + 4);
    drawRect(cx - 5, y + h2 - 14, cx + 5, y + h2 - 10);
  }

  glColor3f(0.95f * d, 0.85f * d, 0.60f * d);
  drawRect(x + 30, y + h2 - 10, x + w2 + 5, y + h2);
  drawRect(x + 30, y + h2, x + w2 + 5, y + h2 + 8);

  drawRectGradientY(x + 10, y, x + 25, y + 15, 0.85f * d, 0.70f * d, 0.45f * d,
                    1.00f * d, 0.85f * d, 0.55f * d);
  drawRectGradientY(x - 20, y, x - 10, y + 25, 0.80f * d, 0.65f * d, 0.40f * d,
                    0.95f * d, 0.75f * d, 0.50f * d);
  drawRectGradientY(x + 5, y, x + 35, y + 8, 0.90f * d, 0.75f * d, 0.50f * d,
                    0.75f * d, 0.60f * d, 0.35f * d);
}

/* ================================================================
 *  MODERN BUILDINGS (Background for Ruined Temples)
 * ================================================================ */
void drawModernBuildings(float x, float y) {
  float d = dimF();

  // Right building (taller, blue-grey with continuous windows)
  float wRight = 65.0f;
  float hRight = 100.0f;
  float xRight = x + 70.0f;

  // Building body
  drawRectGradientY(xRight, y, xRight + wRight, y + hRight, 0.45f * d,
                    0.50f * d, 0.55f * d, 0.65f * d, 0.70f * d, 0.75f * d);

  // Right building floors & continuous windows
  for (int i = 0; i < 4; i++) {
    float fy = y + i * 25.0f;
    if (i > 0) {
      // Windows
      glColor3f(0.20f * d, 0.40f * d, 0.55f * d);
      drawRect(xRight + 2, fy + 5, xRight + wRight - 2, fy + 20);
      // Vertical mullions
      glColor3f(0.15f * d, 0.30f * d, 0.45f * d);
      for (int m = 1; m < 5; m++) {
        float mx = xRight + 2 + m * ((wRight - 4) / 5.0f);
        drawRect(mx - 1, fy + 5, mx + 1, fy + 20);
      }
    } else {
      // Ground floor entrance
      glColor3f(0.25f * d, 0.25f * d, 0.30f * d);
      drawRect(xRight + 20, fy, xRight + 45, fy + 15);
    }
    // Floor separator
    glColor3f(0.75f * d, 0.80f * d, 0.85f * d);
    drawRect(xRight - 2, fy + 20, xRight + wRight + 2, fy + 25);
  }
  // Roof top block
  drawRect(xRight + 15, y + hRight, xRight + 40, y + hRight + 8);

  // Left building (shorter, beige with square windows)
  float wLeft = 65.0f;
  float hLeft = 60.0f;
  float xLeft = x;

  // Building body
  drawRectGradientY(xLeft, y, xLeft + wLeft, y + hLeft, 0.65f * d, 0.60f * d,
                    0.50f * d, 0.80f * d, 0.75f * d, 0.65f * d);

  // Left building floors
  for (int i = 0; i < 2; i++) {
    float fy = y + i * 30.0f;
    if (i == 0) {
      // Ground floor door
      glColor3f(0.35f * d, 0.25f * d, 0.15f * d);
      drawRect(xLeft + 25, fy, xLeft + 40, fy + 18);
      // Ground floor windows
      glColor3f(0.25f * d, 0.45f * d, 0.60f * d);
      drawRect(xLeft + 5, fy + 5, xLeft + 18, fy + 18);
      drawRect(xLeft + 47, fy + 5, xLeft + 60, fy + 18);
    } else {
      // Upper floor windows
      glColor3f(0.25f * d, 0.45f * d, 0.60f * d);
      drawRect(xLeft + 5, fy + 5, xLeft + 18, fy + 20);
      drawRect(xLeft + 25, fy + 5, xLeft + 40, fy + 20);
      drawRect(xLeft + 47, fy + 5, xLeft + 60, fy + 20);

      // Window details
      glColor3f(0.15f * d, 0.30f * d, 0.45f * d);
      drawRect(xLeft + 11, fy + 5, xLeft + 12, fy + 20);
      drawRect(xLeft + 32, fy + 5, xLeft + 33, fy + 20);
      drawRect(xLeft + 53, fy + 5, xLeft + 54, fy + 20);
    }
    // Floor separator
    glColor3f(0.70f * d, 0.65f * d, 0.55f * d);
    drawRect(xLeft - 2, fy + 26, xLeft + wLeft + 2, fy + 30);
  }
}

/* ================================================================
 *  EGYPTIAN HOUSES (Mud-brick flat roof style)
 * ================================================================ */
void drawEgyptianHouse(float x, float y, float w, float h) {
  float d = dimF();

  // Main building body
  drawRectGradientY(x, y, x + w, y + h, 0.75f * d, 0.65f * d, 0.40f * d,
                    0.85f * d, 0.75f * d, 0.50f * d);

  // Roof border / parapet
  glColor3f(0.65f * d, 0.55f * d, 0.35f * d);
  drawRect(x - 2, y + h, x + w + 2, y + h + 4);

  // Door
  glColor3f(0.35f * d, 0.25f * d, 0.15f * d);
  drawRect(x + w * 0.4f, y, x + w * 0.6f, y + h * 0.35f);

  // Door frame
  glColor3f(0.50f * d, 0.40f * d, 0.20f * d);
  drawRect(x + w * 0.4f - 2, y, x + w * 0.4f, y + h * 0.35f + 2);
  drawRect(x + w * 0.6f, y, x + w * 0.6f + 2, y + h * 0.35f + 2);
  drawRect(x + w * 0.4f - 2, y + h * 0.35f, x + w * 0.6f + 2,
           y + h * 0.35f + 2);

  // Small windows
  glColor3f(0.20f * d, 0.15f * d, 0.10f * d);
  drawRect(x + w * 0.15f, y + h * 0.6f, x + w * 0.3f, y + h * 0.75f);
  drawRect(x + w * 0.7f, y + h * 0.6f, x + w * 0.85f, y + h * 0.75f);

  // Window sills
  glColor3f(0.55f * d, 0.45f * d, 0.25f * d);
  drawRect(x + w * 0.15f - 1, y + h * 0.6f - 2, x + w * 0.3f + 1, y + h * 0.6f);
  drawRect(x + w * 0.7f - 1, y + h * 0.6f - 2, x + w * 0.85f + 1, y + h * 0.6f);
}

/* ================================================================
 *  EGYPTIAN 3-STORY HOUSE (Multi-level mud-brick building)
 * ================================================================ */
void drawEgyptian3StoryHouse(float x, float y, float w, float floorH) {
  float d = dimF();
  int numFloors = 3;

  for (int i = 0; i < numFloors; i++) {
    float fy = y + i * floorH;
    // floor body
    drawRectGradientY(x, fy, x + w, fy + floorH, 0.75f * d, 0.65f * d,
                      0.40f * d, 0.85f * d, 0.75f * d, 0.50f * d);
    // cornice
    glColor3f(0.65f * d, 0.55f * d, 0.35f * d);
    drawRect(x - 2, fy + floorH, x + w + 2, fy + floorH + 4);

    if (i == 0) {
      // Ground floor door
      glColor3f(0.35f * d, 0.25f * d, 0.15f * d);
      drawRect(x + w * 0.35f, fy, x + w * 0.65f, fy + floorH * 0.45f);
      // Door frame
      glColor3f(0.50f * d, 0.40f * d, 0.20f * d);
      drawRect(x + w * 0.35f - 2, fy, x + w * 0.35f, fy + floorH * 0.45f + 2);
      drawRect(x + w * 0.65f, fy, x + w * 0.65f + 2, fy + floorH * 0.45f + 2);
      drawRect(x + w * 0.35f - 2, fy + floorH * 0.45f, x + w * 0.65f + 2,
               fy + floorH * 0.45f + 2);
    } else {
      // Upper floor windows
      glColor3f(0.20f * d, 0.15f * d, 0.10f * d);
      drawRect(x + w * 0.15f, fy + floorH * 0.25f, x + w * 0.35f,
               fy + floorH * 0.65f);
      drawRect(x + w * 0.65f, fy + floorH * 0.25f, x + w * 0.85f,
               fy + floorH * 0.65f);
      // Window sills
      glColor3f(0.55f * d, 0.45f * d, 0.25f * d);
      drawRect(x + w * 0.15f - 1, fy + floorH * 0.25f - 2, x + w * 0.35f + 1,
               fy + floorH * 0.25f);
      drawRect(x + w * 0.65f - 1, fy + floorH * 0.25f - 2, x + w * 0.85f + 1,
               fy + floorH * 0.25f);
    }
  }

  // Roof access block
  float roofY = y + numFloors * floorH + 4;
  drawRectGradientY(x + w * 0.1f, roofY, x + w * 0.45f, roofY + 12, 0.75f * d,
                    0.65f * d, 0.40f * d, 0.85f * d, 0.75f * d, 0.50f * d);
}

/* ================================================================
 *  VERTICAL PERSPECTIVE ROAD / BRIDGE
 * ================================================================ */
void drawPerspectiveRoad(float cx) {
  float d = dimF();
  float botY = 140.0f;
  float topY = 445.0f;
  float botW = 120.0f;
  float topW = 45.0f;

  if (sceneMode == 2)
    glColor3f(0.20f, 0.20f, 0.22f);
  else
    glColor3f(0.35f, 0.35f, 0.38f);
  drawQuad(cx - botW / 2, botY, cx + botW / 2, botY, cx + topW / 2, topY,
           cx - topW / 2, topY);

  glColor3f(0.40f * d, 0.30f * d, 0.18f * d);
  drawQuad(cx - botW / 2 - 12, botY, cx - botW / 2, botY, cx - topW / 2, topY,
           cx - topW / 2 - 6, topY);
  drawQuad(cx + botW / 2, botY, cx + botW / 2 + 12, botY, cx + topW / 2 + 6,
           topY, cx + topW / 2, topY);

  glColor3f(0.85f * d, 0.75f * d, 0.20f * d);
  for (int i = 0; i < 12; i += 2) {
    float f1 = i / 12.0f;
    float f2 = (i + 0.9f) / 12.0f;
    float cy1 = botY + (topY - botY) * f1;
    float cy2 = botY + (topY - botY) * f2;
    float cw1 = 3.5f * (1.0f - 0.6f * f1);
    float cw2 = 3.5f * (1.0f - 0.6f * f2);
    drawQuad(cx - cw1, cy1, cx + cw1, cy1, cx + cw2, cy2, cx - cw2, cy2);
  }
}

/* ================================================================
 *  CONSTRUCTION CRANE with rotating arm and warning light
 * ================================================================ */
void drawCrane(float x, float y) {
  int i;
  float ax;

  glColor3f(0.85f, 0.65f, 0.10f);
  drawRect(x - 14, y, x + 14, y + 10);

  drawRect(x - 5, y + 10, x + 5, y + 115);

  glColor3f(0.75f, 0.55f, 0.05f);
  for (i = 0; i < 5; i++) {
    float yy = y + 10 + i * 20;
    drawLine(x - 5, yy, x + 5, yy + 20);
    drawLine(x + 5, yy, x - 5, yy + 20);
  }

  glColor3f(0.80f, 0.60f, 0.08f);
  drawRect(x - 10, y + 110, x + 10, y + 120);

  glColor3f(0.85f, 0.65f, 0.10f);
  ax = x + 75.0f * cosf(craneAng * PI / 180.0f);
  drawRect(x - 20, y + 118, ax, y + 122);

  drawRect(x - 30, y + 118, x - 20, y + 122);
  glColor3f(0.50f, 0.40f, 0.05f);
  drawRect(x - 35, y + 114, x - 28, y + 122);

  glColor3f(0.30f, 0.30f, 0.30f);
  drawLine(ax, y + 118, ax, y + 90);

  glColor3f(0.40f, 0.40f, 0.40f);
  drawLine(ax - 4, y + 90, ax + 4, y + 90);
  drawLine(ax - 4, y + 90, ax - 4, y + 86);
  drawLine(ax + 4, y + 90, ax + 4, y + 86);
  drawArc(ax, y + 84, 4, PI, 2 * PI, 10);

  glColor3f(0.40f, 0.40f, 0.40f);
  drawLine(x, y + 125, ax, y + 122);
  drawLine(x, y + 125, x - 30, y + 122);

  if ((sceneMode == 2)) {
    float blink = sinf(sunG * 8.0f);
    if (blink > 0) {
      glColor3f(1.0f, 0.0f, 0.0f);
      drawFilledCircle(x, y + 128, 4, 10);

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(1.0f, 0.0f, 0.0f, 0.08f);
      drawFilledCircle(x, y + 128, 15, 15);
      glDisable(GL_BLEND);
    }
  }
}

/* ================================================================
 *  CONSTRUCTION SITE (Crane + Under-construction building)
 * ================================================================ */
void drawConstructionSite(float x, float y) {
  float d = dimF();

  glColor3f(0.6f * d, 0.45f * d, 0.25f * d);
  drawRect(x - 30, y, x + 160, y + 25);

  drawCrane(x, y);

  float ax = x + 75.0f * cosf(craneAng * PI / 180.0f);
  float blockY = y + 68.0f;

  glColor3f(0.5f * d, 0.5f * d, 0.52f * d);
  drawRect(ax - 15, blockY, ax + 15, blockY + 8);
  glColor3f(0.3f, 0.3f, 0.3f);
  drawLine(ax, y + 84, ax, blockY + 8);
  drawLine(ax, y + 84, ax - 10, blockY + 8);
  drawLine(ax, y + 84, ax + 10, blockY + 8);

  float bx = x + 60;
  float by = y;

  glColor3f(0.65f * d, 0.65f * d, 0.68f * d);
  drawRect(bx, by, bx + 80, by + 25);
  drawRect(bx, by + 25, bx + 80, by + 50);
  drawRect(bx, by + 50, bx + 80, by + 55);

  glColor3f(0.5f * d, 0.5f * d, 0.53f * d);
  for (int i = 0; i < 6; i++) {
    float px = bx + 4 + i * 14;
    drawRect(px, by + 55, px + 4, by + 90);
  }

  glColor3f(0.2f * d, 0.2f * d, 0.22f * d);
  for (int level = 0; level < 2; level++) {
    float wiley = by + 6 + level * 25;
    for (int i = 0; i < 4; i++) {
      float wilex = bx + 8 + i * 18;
      drawRect(wilex, wiley, wilex + 10, wiley + 12);
    }
  }

  glColor3f(0.65f * d, 0.5f * d, 0.28f * d);
  drawRect(x - 30, y - 10, x + 40, y + 10);
  drawRect(x + 70, y - 10, x + 160, y + 10);
}

/* ================================================================
 *  WIND TURBINE with rotating blades
 * ================================================================ */
void drawWindTurbine(float x, float y) {
  int i;
  float a, bx, by;

  glColor3f(0.70f, 0.70f, 0.72f);
  drawRect(x - 8, y, x + 8, y + 5);

  glColor3f(0.85f, 0.85f, 0.88f);
  drawQuad(x - 4, y + 5, x + 4, y + 5, x + 3, y + 95, x - 3, y + 95);

  glColor3f(0.80f, 0.80f, 0.82f);
  drawFilledEllipse(x, y + 95, 8, 5, 15);

  glColor3f(0.70f, 0.70f, 0.72f);
  drawFilledCircle(x, y + 95, 5, 12);

  glColor3f(0.92f, 0.92f, 0.94f);
  for (i = 0; i < 3; i++) {
    a = turbAng + (float)i * 120.0f;
    bx = x + 38.0f * cosf(a * PI / 180.0f);
    by = y + 95.0f + 38.0f * sinf(a * PI / 180.0f);

    glBegin(GL_TRIANGLES);
    glVertex2f(x + 3.5f * cosf((a + 90) * PI / 180),
               y + 95 + 3.5f * sinf((a + 90) * PI / 180));
    glVertex2f(x - 3.5f * cosf((a + 90) * PI / 180),
               y + 95 - 3.5f * sinf((a + 90) * PI / 180));
    glVertex2f(bx, by);
    glEnd();

    glColor3f(0.80f, 0.80f, 0.82f);
    drawLine(x, y + 95, bx, by);
    glColor3f(0.92f, 0.92f, 0.94f);
  }

  glColor3f(0.50f, 0.50f, 0.52f);
  drawFilledCircle(x, y + 95, 2, 8);
}

/* ================================================================
 *  VEHICLES - Cars, Bus, Truck, Motorcycle
 * ================================================================ */

void drawVerticalCar(float r, float g, float b, int dir) {
  float w = 12.0f;
  float h = 18.0f;

  if (sceneMode == 2) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
    drawRect(-w - 3, -h - 3, w + 3, h + 3);
    glDisable(GL_BLEND);
  }

  glColor3f(r, g, b);
  drawRect(-w, -h, w, h);

  glColor3f(r * 0.8f, g * 0.8f, b * 0.8f);
  drawRect(-w + 2, -h + 8, w - 2, h - 8);

  glColor3f(0.1f, 0.15f, 0.2f);
  if (dir == 1) {
    drawRect(-w + 2, h - 8, w - 2, h - 4);
    drawRect(-w + 2, -h + 4, w - 2, -h + 8);
  } else {
    drawRect(-w + 2, -h + 4, w - 2, -h + 8);
    drawRect(-w + 2, h - 8, w - 2, h - 4);
  }

  if (sceneMode == 2)
    glColor3f(1.0f, 1.0f, 0.6f);
  else
    glColor3f(0.9f, 0.9f, 0.8f);

  if (dir == 1) {
    drawRect(-w + 2, h - 2, -w + 6, h);
    drawRect(w - 6, h - 2, w - 2, h);

    if (sceneMode == 2) {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(1.0f, 1.0f, 0.6f, 0.15f);
      drawTri(-w + 4, h, -w - 20, h + 40, w + 20, h + 40);
      glDisable(GL_BLEND);
    }
  } else {
    drawRect(-w + 2, -h, -w + 6, -h + 2);
    drawRect(w - 6, -h, w - 2, -h + 2);

    if (sceneMode == 2) {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(1.0f, 1.0f, 0.6f, 0.15f);
      drawTri(-w + 4, -h, -w - 20, -h - 40, w + 20, -h - 40);
      glDisable(GL_BLEND);
    }
  }
}

void drawCar(float x, float y, float r, float g, float b, int dir) {

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
  drawFilledEllipse(x + 29, y - 2, 33, 4, 20);
  glDisable(GL_BLEND);

  glColor3f(r, g, b);
  drawRect(x, y, x + 58, y + 18);

  glColor3f(r * 0.85f, g * 0.85f, b * 0.85f);
  drawQuad(x + 10, y + 18, x + 48, y + 18, x + 44, y + 32, x + 14, y + 32);

  glColor3f(0.55f, 0.80f, 1.0f);
  if ((sceneMode == 2))
    glColor3f(0.35f, 0.55f, 0.75f);
  drawQuad(x + 32, y + 19, x + 47, y + 19, x + 43, y + 30, x + 34, y + 30);

  drawQuad(x + 12, y + 19, x + 27, y + 19, x + 25, y + 30, x + 15, y + 30);

  glColor3f(r * 0.75f, g * 0.75f, b * 0.75f);
  drawRect(x + 28, y + 19, x + 31, y + 30);

  glColor3f(0.30f, 0.30f, 0.32f);
  drawRect(x - 2, y + 4, x, y + 14);
  drawRect(x + 58, y + 4, x + 60, y + 14);

  glColor3f(r * 0.70f, g * 0.70f, b * 0.70f);
  drawLine(x + 29, y + 2, x + 29, y + 18);

  glColor3f(0.40f, 0.40f, 0.42f);
  drawRect(x + 22, y + 10, x + 26, y + 12);
  drawRect(x + 36, y + 10, x + 40, y + 12);

  glColor3f(0.12f, 0.12f, 0.12f);
  drawFilledCircle(x + 14, y, 8, 16);
  drawFilledCircle(x + 44, y, 8, 16);

  glColor3f(0.55f, 0.55f, 0.55f);
  drawFilledCircle(x + 14, y, 4, 12);
  drawFilledCircle(x + 44, y, 4, 12);

  glColor3f(0.35f, 0.35f, 0.35f);
  drawFilledCircle(x + 14, y, 1.5f, 8);
  drawFilledCircle(x + 44, y, 1.5f, 8);

  glColor3f(0.20f, 0.20f, 0.20f);
  drawCircleOutline(x + 14, y, 8, 16);
  drawCircleOutline(x + 44, y, 8, 16);

  if ((sceneMode == 2)) {
    if (dir > 0) {

      glColor3f(1.0f, 1.0f, 0.50f);
      drawFilledCircle(x + 58, y + 12, 3, 10);
      drawFilledCircle(x + 58, y + 6, 3, 10);

      glColor3f(1.0f, 0.0f, 0.0f);
      drawFilledCircle(x + 1, y + 12, 2, 8);

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(1.0f, 1.0f, 0.50f, 0.04f);
      drawTri(x + 58, y + 9, x + 120, y - 5, x + 120, y + 25);
      glDisable(GL_BLEND);
    } else {
      glColor3f(1.0f, 1.0f, 0.50f);
      drawFilledCircle(x + 1, y + 12, 3, 10);
      drawFilledCircle(x + 1, y + 6, 3, 10);
      glColor3f(1.0f, 0.0f, 0.0f);
      drawFilledCircle(x + 57, y + 12, 2, 8);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(1.0f, 1.0f, 0.50f, 0.04f);
      drawTri(x + 1, y + 9, x - 60, y - 5, x - 60, y + 25);
      glDisable(GL_BLEND);
    }
  } else {

    glColor3f(0.90f, 0.90f, 0.70f);
    drawFilledCircle(x + 57, y + 10, 3, 8);
    drawFilledCircle(x + 2, y + 10, 3, 8);
  }

  glColor3f(1.0f, 1.0f, 1.0f);
  if (dir > 0)
    drawRect(x + 54, y + 2, x + 60, y + 5);
  else
    drawRect(x, y + 2, x + 6, y + 5);
}

void drawBus(float x, float y) {
  int i;
  float d = dimF();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
  drawFilledEllipse(x + 50, y - 2, 55, 6, 25);
  glDisable(GL_BLEND);

  glColor3f(0.95f, 0.75f, 0.10f);
  drawRect(x, y, x + 100, y + 38);

  glColor3f(0.90f, 0.70f, 0.08f);
  drawRect(x, y + 38, x + 100, y + 42);

  glColor3f(0.85f, 0.65f, 0.05f);
  drawRect(x, y + 14, x + 100, y + 17);

  for (i = 0; i < 5; i++) {
    float wx = x + 6 + i * 18;

    if ((sceneMode == 2))
      glColor3f(0.90f, 0.85f, 0.60f);
    else
      glColor3f(0.55f, 0.82f, 1.0f);
    drawRect(wx, y + 20, wx + 13, y + 35);

    glColor3f(0.40f, 0.30f, 0.05f);
    drawLine(wx, y + 20, wx, y + 35);
    drawLine(wx + 13, y + 20, wx + 13, y + 35);
    drawLine(wx, y + 20, wx + 13, y + 20);
    drawLine(wx, y + 35, wx + 13, y + 35);
  }

  glColor3f(0.10f, 0.10f, 0.10f);
  drawRect(x + 85, y + 28, x + 97, y + 36);

  glColor3f(0.80f, 0.60f, 0.05f);
  drawRect(x + 94, y + 2, x + 100, y + 17);
  drawLine(x + 97, y + 2, x + 97, y + 17);

  glColor3f(0.12f, 0.12f, 0.12f);
  drawFilledCircle(x + 18, y, 10, 18);
  drawFilledCircle(x + 80, y, 10, 18);
  glColor3f(0.50f, 0.50f, 0.50f);
  drawFilledCircle(x + 18, y, 5, 12);
  drawFilledCircle(x + 80, y, 5, 12);
  glColor3f(0.30f, 0.30f, 0.30f);
  drawFilledCircle(x + 18, y, 2, 8);
  drawFilledCircle(x + 80, y, 2, 8);

  if ((sceneMode == 2)) {
    glColor3f(1.0f, 1.0f, 0.50f);
    drawFilledCircle(x, y + 10, 3, 10);
    drawFilledCircle(x, y + 5, 3, 10);
  }

  if ((sceneMode == 2)) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 0.95f, 0.70f, 0.08f);
    drawRect(x + 2, y + 18, x + 98, y + 37);
    glDisable(GL_BLEND);
  }
}

void drawTruck(float x, float y) {

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
  drawFilledEllipse(x + 47, y - 2, 52, 6, 25);
  glDisable(GL_BLEND);

  glColor3f(0.20f, 0.65f, 0.20f);
  drawRect(x, y + 2, x + 68, y + 42);

  glColor3f(0.18f, 0.58f, 0.18f);
  drawLine(x, y + 15, x + 68, y + 15);
  drawLine(x, y + 28, x + 68, y + 28);

  glColor3f(0.40f, 0.40f, 0.40f);
  drawRect(x + 32, y + 5, x + 36, y + 15);

  glColor3f(0.25f, 0.72f, 0.25f);
  drawRect(x + 68, y, x + 95, y + 34);

  glColor3f(0.22f, 0.65f, 0.22f);
  drawRect(x + 68, y + 34, x + 95, y + 38);

  if ((sceneMode == 2))
    glColor3f(0.35f, 0.55f, 0.75f);
  else
    glColor3f(0.55f, 0.82f, 1.0f);
  drawRect(x + 72, y + 14, x + 92, y + 30);

  glColor3f(0.30f, 0.30f, 0.30f);
  drawLine(x + 82, y + 14, x + 78, y + 28);

  glColor3f(0.30f, 0.30f, 0.32f);
  drawRect(x + 95, y + 22, x + 99, y + 26);

  glColor3f(0.12f, 0.12f, 0.12f);
  drawFilledCircle(x + 15, y, 10, 18);
  drawFilledCircle(x + 52, y, 10, 18);
  drawFilledCircle(x + 80, y, 10, 18);
  glColor3f(0.50f, 0.50f, 0.50f);
  drawFilledCircle(x + 15, y, 5, 12);
  drawFilledCircle(x + 52, y, 5, 12);
  drawFilledCircle(x + 80, y, 5, 12);

  if ((sceneMode == 2)) {
    glColor3f(1.0f, 1.0f, 0.50f);
    drawFilledCircle(x + 95, y + 10, 3, 10);
    glColor3f(1.0f, 0.5f, 0.0f);
    drawFilledCircle(x + 95, y + 18, 2, 8);
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(0.50f, 0.50f, 0.50f, 0.15f);
  drawFilledCircle(x - 5, y + 38, 5, 10);
  drawFilledCircle(x - 12, y + 42, 7, 10);
  drawFilledCircle(x - 22, y + 45, 9, 10);
  glDisable(GL_BLEND);
}

void drawMotorcycle(float x, float y) {

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
  drawFilledEllipse(x + 15, y - 2, 18, 3, 15);
  glDisable(GL_BLEND);

  glColor3f(0.12f, 0.12f, 0.12f);
  drawFilledCircle(x, y, 7, 14);
  drawFilledCircle(x + 30, y, 7, 14);
  glColor3f(0.45f, 0.45f, 0.45f);
  drawFilledCircle(x, y, 3, 10);
  drawFilledCircle(x + 30, y, 3, 10);

  glColor3f(0.70f, 0.10f, 0.10f);
  drawQuad(x + 5, y + 3, x + 25, y + 3, x + 22, y + 12, x + 8, y + 12);

  glColor3f(0.15f, 0.15f, 0.15f);
  drawRect(x + 8, y + 12, x + 20, y + 15);

  glColor3f(0.50f, 0.50f, 0.52f);
  drawRect(x + 12, y + 3, x + 18, y + 8);

  glColor3f(0.40f, 0.40f, 0.42f);
  drawLine(x + 22, y + 12, x + 28, y + 18);
  drawLine(x + 26, y + 18, x + 30, y + 18);

  if ((sceneMode == 2)) {
    glColor3f(1.0f, 1.0f, 0.50f);
    drawFilledCircle(x + 30, y + 8, 2, 8);
  }

  glColor3f(1.0f, 0.0f, 0.0f);
  drawFilledCircle(x + 2, y + 8, 1.5f, 6);
}

/* ================================================================
 *  CRUISE SHIP
 * ================================================================ */
void drawCruiseShip(float x, float y) {
  float d = dimF();

  // Hull (lower)
  glColor3f(0.15f * d, 0.20f * d, 0.40f * d);
  drawQuad(x - 90, y, x + 80, y, x + 110, y + 25, x - 100, y + 25);

  // Red waterline stripe
  glColor3f(0.85f * d, 0.20f * d, 0.25f * d);
  drawRect(x - 99, y + 25, x + 109, y + 27);

  // Deck 1 (White)
  glColor3f(0.95f * d, 0.95f * d, 0.95f * d);
  drawQuad(x - 95, y + 27, x + 105, y + 27, x + 100, y + 45, x - 90, y + 45);

  // Deck 2
  glColor3f(0.90f * d, 0.90f * d, 0.92f * d);
  drawQuad(x - 85, y + 45, x + 95, y + 45, x + 90, y + 60, x - 80, y + 60);

  // Deck 3 (Bridge / Top)
  glColor3f(0.85f * d, 0.85f * d, 0.88f * d);
  drawQuad(x - 60, y + 60, x + 70, y + 60, x + 65, y + 75, x - 55, y + 75);

  // Windows Deck 1 & 2
  glColor3f(0.2f * d, 0.4f * d, 0.8f * d);
  for (int i = 0; i < 15; i++) {
    drawFilledCircle((x - 80) + i * 12, y + 36, 3.0f, 8); // Deck 1
    if (i < 13) drawFilledCircle((x - 70) + i * 12, y + 52, 2.5f, 8); // Deck 2
  }

  // Bridge windows
  glColor3f(0.1f * d, 0.2f * d, 0.4f * d);
  drawRect(x - 45, y + 65, x + 50, y + 70);

  // Funnel (Chimney)
  glColor3f(0.8f * d, 0.2f * d, 0.2f * d);
  drawQuad(x - 30, y + 75, x - 10, y + 75, x - 5, y + 100, x - 25, y + 100);
  glColor3f(0.1f * d, 0.1f * d, 0.1f * d);
  drawRect(x - 26, y + 90, x - 5, y + 100);

  // Smoke from Funnel
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  float t = camelAnimTime * 3.0f;
  for (int i = 0; i < 10; i++) {
     float offset = i * 2.5f;
     float age = fmodf(t + offset, 25.0f);
     if (age > 0.0f) {
        float driftX = (x - 15) + (age * 1.5f); // smoke drifts right
        float driftY = y + 100 + (age * 2.0f); // moves up
        float sSize = 3.0f + age * 0.7f; // expands
        float sAlpha = 0.5f * (1.0f - (age / 25.0f)); // fades out

        // Outer lighter smoke
        glColor4f(0.6f, 0.6f, 0.65f, sAlpha);
        drawFilledCircle(driftX, driftY, sSize, 12);
        // Inner darker smoke
        glColor4f(0.4f, 0.4f, 0.45f, sAlpha * 1.5f);
        drawFilledCircle(driftX, driftY, sSize * 0.6f, 8);
     }
  }
  glDisable(GL_BLEND);

  // Night lights
  if (sceneMode == 2) {
    glColor3f(1.0f, 1.0f, 0.6f);
    for (int i = 0; i < 15; i++) {
      if ((i+cruiseWaitTimer)%3 != 0) { // Some windows lit
        drawFilledCircle((x - 80) + i * 12, y + 36, 3.0f, 8);
      }
      if (i < 13 && (i*2)%3 != 0) {
        drawFilledCircle((x - 70) + i * 12, y + 52, 2.5f, 8);
      }
    }
    glColor3f(1.0f, 0.0f, 0.0f); drawFilledCircle(x - 90, y + 45, 2.5f, 8);
    glColor3f(0.0f, 1.0f, 0.0f); drawFilledCircle(x + 100, y + 45, 2.5f, 8);
  }
}

void drawSailboat(float x, float y) {
  float d = dimF();

  glColor3f(0.60f * d, 0.38f * d, 0.18f * d);
  drawQuad(x - 20, y, x + 20, y, x + 15, y - 10, x - 15, y - 10);

  glColor3f(0.50f * d, 0.30f * d, 0.12f * d);
  drawQuad(x - 18, y - 2, x + 18, y - 2, x + 15, y - 10, x - 15, y - 10);

  glColor3f(0.52f * d, 0.32f * d, 0.12f * d);
  glLineWidth(2.0f);
  drawLine(x, y, x, y + 38);
  glLineWidth(1.0f);

  glColor3f(1.0f * d, 1.0f * d, 0.95f * d);
  drawTri(x, y + 38, x, y + 6, x + 22, y + 10);

  glColor3f(0.95f * d, 0.95f * d, 0.90f * d);
  drawTri(x, y + 35, x, y + 12, x - 12, y + 14);

  glColor3f(0.85f * d, 0.25f * d, 0.15f * d);
  drawLine(x + 5, y + 25, x + 15, y + 12);

  if ((sceneMode == 2)) {
    glColor3f(1.0f, 0.80f, 0.30f);
    drawFilledCircle(x, y + 40, 3, 10);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 0.80f, 0.30f, 0.06f);
    drawFilledCircle(x, y + 40, 15, 15);
    glDisable(GL_BLEND);
  }
}

/* ================================================================
 *  HELICOPTER with rotor, spotlight, and hovering animation
 * ================================================================ */
void drawHelicopter(float x, float y) {
  float rotorLen;

  glColor3f(0.80f, 0.22f, 0.15f);
  drawFilledEllipse(x, y, 28, 14, 25);

  glColor3f(0.50f, 0.75f, 0.95f);
  drawFilledEllipse(x + 15, y + 3, 8, 6, 12);

  glColor3f(0.75f, 0.18f, 0.12f);
  drawQuad(x - 28, y + 3, x - 28, y - 3, x - 60, y + 5, x - 60, y + 9);

  glColor3f(0.80f, 0.22f, 0.15f);
  drawTri(x - 58, y + 9, x - 62, y + 20, x - 55, y + 9);

  glColor3f(0.35f, 0.35f, 0.38f);
  rotorLen = 8.0f + 2.0f * sinf(sunG * 25.0f);
  drawLine(x - 60, y + 15 + rotorLen, x - 60, y + 15 - rotorLen);

  glColor3f(0.35f, 0.35f, 0.38f);
  rotorLen = 45.0f;
  glLineWidth(2.5f);
  float rotorAngle = sunG * 20.0f;
  drawLine(x + rotorLen * cosf(rotorAngle), y + 16,
           x - rotorLen * cosf(rotorAngle), y + 16);
  drawLine(x + rotorLen * cosf(rotorAngle + PI / 2), y + 16,
           x - rotorLen * cosf(rotorAngle + PI / 2), y + 16);
  glLineWidth(1.0f);

  glColor3f(0.40f, 0.40f, 0.42f);
  drawFilledCircle(x, y + 16, 3, 10);

  glColor3f(0.35f, 0.35f, 0.38f);
  glLineWidth(2.0f);
  drawLine(x - 18, y - 16, x + 18, y - 16);
  drawLine(x - 12, y - 16, x - 8, y - 2);
  drawLine(x + 12, y - 16, x + 8, y - 2);
  glLineWidth(1.0f);

  if ((sceneMode == 2)) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 0.70f, 0.10f);
    drawTri(x, y - 16, x - 35, y - 90, x + 35, y - 90);
    glColor4f(1.0f, 1.0f, 0.70f, 0.05f);
    drawTri(x, y - 16, x - 50, y - 90, x + 50, y - 90);
    glDisable(GL_BLEND);
  }
}

/* ================================================================
 *  HOT AIR BALLOON with striped envelope and fire
 * ================================================================ */
void drawBalloon(float x, float y) {

  glColor3f(0.92f, 0.30f, 0.15f);
  drawFilledEllipse(x, y + 32, 28, 38, 35);

  glColor3f(1.0f, 0.85f, 0.20f);
  drawFilledEllipse(x, y + 32, 18, 38, 35);

  glColor3f(0.92f, 0.30f, 0.15f);
  drawFilledEllipse(x, y + 32, 10, 38, 35);

  glColor3f(1.0f, 1.0f, 1.0f);
  drawFilledEllipse(x, y + 32, 4, 38, 35);

  glColor3f(0.58f, 0.38f, 0.18f);
  drawRect(x - 12, y - 12, x + 12, y);

  glColor3f(0.48f, 0.28f, 0.10f);
  drawLine(x - 12, y - 6, x + 12, y - 6);
  drawLine(x - 6, y - 12, x - 6, y);
  drawLine(x, y - 12, x, y);
  drawLine(x + 6, y - 12, x + 6, y);

  glColor3f(0.35f, 0.22f, 0.10f);
  drawLine(x - 12, y, x - 22, y + 10);
  drawLine(x + 12, y, x + 22, y + 10);
  drawLine(x - 12, y, x - 15, y + 15);
  drawLine(x + 12, y, x + 15, y + 15);

  glColor3f(1.0f, 0.65f, 0.10f);
  drawFilledCircle(x, y + 4, 5, 12);
  glColor3f(1.0f, 0.85f, 0.30f);
  drawFilledCircle(x, y + 5, 3, 10);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(1.0f, 0.65f, 0.10f, 0.08f);
  drawFilledCircle(x, y + 4, 18, 15);
  glDisable(GL_BLEND);
}

/* ================================================================
 *  AIRPLANE flying right to left
 * ================================================================ */
void drawAirplane(float x, float y) {
  float d = dimF();

  /* Realistic animated smoke trail */
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  float t = camelAnimTime * 4.0f;
  for (int i = 0; i < 15; i++) {
     float offset = i * 1.5f;
     float age = fmodf(t + offset, 25.0f); // age goes 0 to 25
     if (age > 0.0f) {
        float driftX = x + 30 + age * 4.0f; // drift to right behind plane
        float driftY = y + 8 + sinf(age * 0.4f + i) * (age * 0.15f); // drift up/down
        float sSize = 2.0f + age * 0.5f; // expands
        float sAlpha = 0.6f * (1.0f - (age / 25.0f)); // fades

        // Outer glowing smoke
        glColor4f(0.85f, 0.85f, 0.88f, sAlpha * 0.4f);
        drawFilledCircle(driftX, driftY, sSize * 1.5f, 12);
        // Inner dense smoke
        glColor4f(0.6f, 0.6f, 0.65f, sAlpha);
        drawFilledCircle(driftX, driftY, sSize, 12);
     }
  }
  glDisable(GL_BLEND);

  /* Upper wing (Back wing) */
  glColor3f(0.8f * d, 0.8f * d, 0.8f * d);
  drawQuad(x - 10, y + 5, x + 15, y + 5, x + 30, y + 25, x + 10, y + 25);

  /* Engine Nacelle (Back) */
  glColor3f(0.5f * d, 0.5f * d, 0.5f * d);
  drawFilledEllipse(x + 10, y + 16, 8, 4, 12);

  /* Aerodynamic Fuselage */
  glColor3f(0.95f * d, 0.95f * d, 0.95f * d);
  drawFilledEllipse(x, y + 8, 35, 12, 30);

  /* Blue/Red livery lines */
  glColor3f(0.1f * d, 0.3f * d, 0.7f * d);
  drawRect(x - 30, y + 5, x + 25, y + 7);
  glColor3f(0.9f * d, 0.1f * d, 0.1f * d);
  drawRect(x - 28, y + 3, x + 23, y + 4.5f);

  /* Cockpit window (Left side) */
  glColor3f(0.4f * d, 0.7f * d, 0.9f * d);
  drawFilledEllipse(x - 25, y + 12, 6, 3, 10);
  glColor3f(0.1f * d, 0.2f * d, 0.3f * d);
  drawLine(x - 25, y + 9, x - 22, y + 15);

  /* Passenger windows */
  for (int i = 0; i < 7; i++) {
    glColor3f(0.4f * d, 0.7f * d, 0.9f * d);
    drawFilledCircle(x - 10 + i * 5, y + 9, 1.5f, 8);
    glColor3f(0.2f * d, 0.2f * d, 0.2f * d);
    drawCircleOutline(x - 10 + i * 5, y + 9, 1.5f, 8);
  }

  /* Tail Stabilizer (vertical) */
  glColor3f(0.9f * d, 0.9f * d, 0.9f * d);
  drawQuad(x + 20, y + 12, x + 32, y + 12, x + 38, y + 30, x + 25, y + 30);

  /* Tail logo/coloring */
  glColor3f(0.1f * d, 0.3f * d, 0.7f * d);
  drawTri(x + 25, y + 20, x + 38, y + 30, x + 25, y + 30);

  /* Tail Stabilizer (horizontal) */
  glColor3f(0.85f * d, 0.85f * d, 0.85f * d);
  drawQuad(x + 25, y + 10, x + 33, y + 10, x + 38, y + 15, x + 30, y + 15);

  /* Lower wing (Front wing) */
  glColor3f(0.9f * d, 0.9f * d, 0.9f * d);
  drawQuad(x - 12, y + 2, x + 15, y + 2, x + 28, y - 18, x + 5, y - 18);

  /* Jet Engine under front wing */
  glColor3f(0.6f * d, 0.6f * d, 0.65f * d);
  drawFilledEllipse(x + 5, y - 4, 9, 4, 12);
  /* Intake */
  glColor3f(0.1f * d, 0.1f * d, 0.1f * d);
  drawFilledEllipse(x - 4, y - 4, 2, 4, 8);

  /* Navigation lights at night */
  if (sceneMode == 2) {
    glColor3f(1.0f, 0.0f, 0.0f);
    drawFilledCircle(x + 28, y - 18, 2, 8);
    glColor3f(0.0f, 1.0f, 0.0f);
    drawFilledCircle(x + 30, y + 25, 2, 8);

    // blinking tail light
    float blink = sinf(camelAnimTime * 20.0f);
    if (blink > 0.0f) {
       glColor3f(1.0f, 1.0f, 1.0f);
       drawFilledCircle(x + 38, y + 30, 2, 8);
    }
  }
}

/* ================================================================
 *  NORMAL LEAFY TREE with swaying animation
 * ================================================================ */
void drawTree(float x, float y, float sway) {
  float d = dimF();
  float sx = sway * 15.0f;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
  drawFilledEllipse(x, y - 2, 8, 3, 12);
  glDisable(GL_BLEND);

  drawQuadGradientY(x - 4, y, x + 4, y, x + 2 + sx * 0.2f, y + 45,
                    x - 2 + sx * 0.2f, y + 45, 0.35f * d, 0.22f * d, 0.12f * d,
                    0.45f * d, 0.30f * d, 0.15f * d);

  glColor3f(0.30f * d, 0.18f * d, 0.10f * d);
  drawLine(x - 2, y + 5, x - 1 + sx * 0.15f, y + 35);
  drawLine(x, y + 2, x + 1 + sx * 0.15f, y + 40);

  if ((sceneMode == 2))
    glColor3f(0.08f, 0.18f, 0.08f);
  else
    glColor3f(0.15f * d, 0.45f * d, 0.15f * d);

  float cx = x + sx;
  float cy = y + 55;

  drawFilledCircle(cx - 15, cy - 5, 18, 15);
  drawFilledCircle(cx + 15, cy - 5, 18, 15);
  drawFilledCircle(cx, cy + 10, 22, 18);

  if ((sceneMode == 2))
    glColor3f(0.10f, 0.25f, 0.10f);
  else
    glColor3f(0.20f * d, 0.55f * d, 0.20f * d);

  drawFilledCircle(cx - 10, cy, 16, 15);
  drawFilledCircle(cx + 10, cy, 16, 15);
  drawFilledCircle(cx, cy - 8, 18, 15);

  if ((sceneMode == 2))
    glColor3f(0.12f, 0.30f, 0.12f);
  else
    glColor3f(0.30f * d, 0.65f * d, 0.25f * d);

  drawFilledCircle(cx - 5, cy + 2, 12, 12);
  drawFilledCircle(cx + 5, cy + 4, 14, 12);
  drawFilledCircle(cx, cy + 12, 15, 12);
}

/* ================================================================
 *  CACTUS with arms
 * ================================================================ */
void drawCactus(float x, float y) {
  float d = dimF();

  glColor3f(0.22f * d + 0.08f, 0.55f * d, 0.18f * d + 0.05f);
  drawRect(x - 5, y, x + 5, y + 35);

  glColor3f(0.28f * d + 0.08f, 0.62f * d, 0.22f * d + 0.05f);
  drawRect(x - 2, y + 2, x + 2, y + 33);

  glColor3f(0.22f * d + 0.08f, 0.55f * d, 0.18f * d + 0.05f);
  drawRect(x + 5, y + 12, x + 18, y + 17);
  drawRect(x + 13, y + 17, x + 18, y + 28);

  drawRect(x - 15, y + 18, x - 5, y + 23);
  drawRect(x - 15, y + 17, x - 10, y + 30);

  glColor3f(0.80f * d, 0.80f * d, 0.30f * d);
  glPointSize(1.5f);
  glBegin(GL_POINTS);
  glVertex2f(x - 5, y + 8);
  glVertex2f(x + 5, y + 15);
  glVertex2f(x - 5, y + 20);
  glVertex2f(x + 5, y + 25);
  glVertex2f(x - 5, y + 30);
  glVertex2f(x + 18, y + 22);
  glVertex2f(x - 15, y + 25);
  glVertex2f(x + 5, y + 5);
  glEnd();
  glPointSize(1.0f);

  glColor3f(1.0f * d, 0.30f * d, 0.40f * d);
  drawFilledCircle(x, y + 37, 3, 8);
  glColor3f(1.0f * d, 0.50f * d, 0.20f * d);
  drawFilledCircle(x, y + 37, 1.5f, 6);
}

/* ================================================================
 *  WATER FOUNTAIN with particle animation
 * ================================================================ */
void drawFountain(float x, float y) {
  float d = dimF();
  int i;

  glColor3f(0.62f * d, 0.62f * d, 0.65f * d);
  drawQuad(x - 22, y, x + 22, y, x + 26, y + 10, x - 26, y + 10);

  glColor3f(0.30f * d + 0.1f, 0.60f * d + 0.1f, 0.90f * d);
  drawRect(x - 20, y + 2, x + 20, y + 10);

  glColor3f(0.60f * d, 0.60f * d, 0.63f * d);
  drawRect(x - 5, y + 10, x + 5, y + 22);

  glColor3f(0.62f * d, 0.62f * d, 0.65f * d);
  drawQuad(x - 14, y + 22, x + 14, y + 22, x + 16, y + 28, x - 16, y + 28);

  glColor3f(0.30f * d + 0.1f, 0.60f * d + 0.1f, 0.90f * d);
  drawRect(x - 13, y + 23, x + 13, y + 28);

  glColor3f(0.55f * d, 0.55f * d, 0.58f * d);
  drawRect(x - 2, y + 28, x + 2, y + 34);

  if ((sceneMode == 2))
    glColor3f(0.30f, 0.55f, 0.92f);
  else
    glColor3f(0.45f, 0.72f, 1.0f);
  for (i = 0; i < MAX_FOUNT; i++) {
    drawFilledCircle(x + fntX[i], y + 34 + fntY[i], 1.8f, 6);
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  if ((sceneMode == 2))
    glColor4f(0.30f, 0.55f, 0.92f, 0.15f);
  else
    glColor4f(0.45f, 0.72f, 1.0f, 0.15f);
  drawTri(x - 2, y + 34, x - 10, y + 28, x + 2, y + 34);
  drawTri(x - 2, y + 34, x + 10, y + 28, x + 2, y + 34);
  glDisable(GL_BLEND);

  if ((sceneMode == 2)) {
    glColor3f(0.2f, 0.5f, 1.0f);
    drawFilledCircle(x - 10, y + 10, 2, 6);
    glColor3f(0.2f, 1.0f, 0.3f);
    drawFilledCircle(x, y + 10, 2, 6);
    glColor3f(1.0f, 0.2f, 0.3f);
    drawFilledCircle(x + 10, y + 10, 2, 6);
  }
}

/* ================================================================
 *  DESERT OASIS (Pond, Trees, Rocks, Fountain)
 * ================================================================ */
void drawOasis(float cx, float cy) {
  float d = dimF();

  if (sceneMode == 2)
    glColor3f(0.25f, 0.20f, 0.15f);
  else
    glColor3f(0.55f * d, 0.45f * d, 0.25f * d);
  drawFilledEllipse(cx, cy - 2, 146, 48, 30);

  if (sceneMode == 2)
    glColor3f(0.15f, 0.25f, 0.40f);
  else
    glColor3f(0.20f * d, 0.60f * d, 0.85f * d);
  drawFilledEllipse(cx, cy, 140, 45, 30);

  if (sceneMode == 2)
    glColor3f(0.20f, 0.18f, 0.15f);
  else
    glColor3f(0.48f * d, 0.42f * d, 0.35f * d);
  drawFilledEllipse(cx - 100, cy - 30, 20, 8, 12);
  drawFilledEllipse(cx + 80, cy - 35, 25, 9, 12);
  drawFilledEllipse(cx + 120, cy + 10, 15, 6, 12);
  drawFilledEllipse(cx - 110, cy + 20, 18, 7, 12);

  drawTree(cx - 80, cy + 20, 0.0f);
  drawTree(cx + 110, cy + 15, 0.0f);

  drawFountain(cx, cy);

  drawTree(cx - 120, cy - 10, 0.0f);
  drawTree(cx + 90, cy - 20, 0.0f);
}

/* ================================================================
 *  BILLBOARD / Signs
 * ================================================================ */
void drawBillboard(float x, float y, const char *text) {

  glColor3f(0.42f, 0.42f, 0.45f);
  drawRect(x + 5, y, x + 8, y + 40);
  drawRect(x + 58, y, x + 61, y + 40);

  glColor3f(1.0f, 1.0f, 1.0f);
  drawRect(x, y + 40, x + 66, y + 62);

  glColor3f(0.25f, 0.25f, 0.28f);
  glLineWidth(1.5f);
  glBegin(GL_LINE_LOOP);
  glVertex2f(x, y + 40);
  glVertex2f(x + 66, y + 40);
  glVertex2f(x + 66, y + 62);
  glVertex2f(x, y + 62);
  glEnd();
  glLineWidth(1.0f);

  glColor3f(0.85f, 0.70f, 0.35f);
  drawTri(x + 5, y + 44, x + 20, y + 44, x + 12.5f, y + 56);

  if ((sceneMode == 2)) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 0.80f, 0.10f);
    drawRect(x - 3, y + 37, x + 69, y + 65);
    glDisable(GL_BLEND);

    glColor3f(0.90f, 0.90f, 0.50f);
    drawFilledCircle(x + 15, y + 40, 2, 6);
    drawFilledCircle(x + 50, y + 40, 2, 6);
  }
}

/* ================================================================
 *  TRAFFIC LIGHT with Red/Yellow/Green cycle
 * ================================================================ */
void drawTrafficLight(float x, float y) {

  glColor3f(0.32f, 0.32f, 0.35f);
  drawRect(x - 2, y, x + 2, y + 52);

  glColor3f(0.15f, 0.15f, 0.18f);
  drawRect(x - 8, y + 52, x + 8, y + 82);

  glColor3f(0.25f, 0.25f, 0.28f);
  glBegin(GL_LINE_LOOP);
  glVertex2f(x - 8, y + 52);
  glVertex2f(x + 8, y + 52);
  glVertex2f(x + 8, y + 82);
  glVertex2f(x - 8, y + 82);
  glEnd();

  glColor3f(trafSt == 0 ? 1.0f : 0.25f, 0.0f, 0.0f);
  drawFilledCircle(x, y + 74, 4.5f, 12);

  if (trafSt == 0) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 0.0f, 0.0f, 0.08f);
    drawFilledCircle(x, y + 74, 10, 15);
    glDisable(GL_BLEND);
  }

  glColor3f(trafSt == 1 ? 1.0f : 0.25f, trafSt == 1 ? 0.80f : 0.12f, 0.0f);
  drawFilledCircle(x, y + 65, 4.5f, 12);

  glColor3f(0.0f, trafSt == 2 ? 0.90f : 0.18f, 0.0f);
  drawFilledCircle(x, y + 56, 4.5f, 12);
  if (trafSt == 2) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.90f, 0.0f, 0.08f);
    drawFilledCircle(x, y + 56, 10, 15);
    glDisable(GL_BLEND);
  }

  glColor3f(0.12f, 0.12f, 0.15f);
  drawRect(x - 9, y + 78, x + 9, y + 80);
  drawRect(x - 9, y + 69, x + 9, y + 71);
  drawRect(x - 9, y + 60, x + 9, y + 62);
}

/* ================================================================
 *  STREET LAMP with light cone at night
 * ================================================================ */
void drawStreetLamp(float x, float y) {

  glColor3f(0.38f, 0.38f, 0.40f);
  drawRect(x - 2, y, x + 2, y + 62);

  drawQuad(x + 2, y + 58, x + 20, y + 55, x + 20, y + 57, x + 2, y + 62);

  glColor3f(0.30f, 0.30f, 0.32f);
  drawRect(x + 15, y + 52, x + 25, y + 55);

  if ((sceneMode == 2)) {
    glColor3f(1.0f, 0.92f, 0.55f);
    drawFilledCircle(x + 20, y + 52, 5, 12);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 0.92f, 0.55f, 0.06f);
    drawTri(x + 20, y + 47, x + 5, y, x + 35, y);
    glColor4f(1.0f, 0.92f, 0.55f, 0.03f);
    drawTri(x + 20, y + 47, x - 5, y, x + 45, y);
    glDisable(GL_BLEND);
  } else {
    glColor3f(0.82f, 0.82f, 0.62f);
    drawFilledCircle(x + 20, y + 52, 5, 12);
  }
}

/* ================================================================
 *  SOLAR PANELS
 * ================================================================ */
void drawSolarPanel(float x, float y) {
  float d = dimF();

  if ((sceneMode == 2))
    glColor3f(0.15f, 0.15f, 0.28f);
  else
    glColor3f(0.15f, 0.22f, 0.52f);
  drawQuad(x, y, x + 50, y, x + 45, y + 25, x + 5, y + 25);

  glColor3f(0.30f * d, 0.30f * d, 0.60f * d);
  for (int i = 1; i < 5; i++) {
    float lx = x + (float)i * 10;
    drawLine(lx, y, lx + 1, y + 25);
  }
  for (int i = 1; i < 3; i++) {
    drawLine(x + 3, y + i * 8, x + 48, y + i * 8);
  }

  glColor3f(0.50f * d, 0.50f * d, 0.52f * d);
  glBegin(GL_LINE_LOOP);
  glVertex2f(x, y);
  glVertex2f(x + 50, y);
  glVertex2f(x + 45, y + 25);
  glVertex2f(x + 5, y + 25);
  glEnd();

  glColor3f(0.42f, 0.42f, 0.44f);
  drawLine(x + 25, y, x + 25, y - 10);
  drawLine(x + 20, y - 10, x + 30, y - 10);

  if (!(sceneMode == 2)) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, 0.12f);
    drawTri(x + 5, y + 2, x + 15, y + 22, x + 25, y + 2);
    glDisable(GL_BLEND);
  }
}

/* ================================================================
 *  POWER LINE TOWER
 * ================================================================ */
void drawPowerTower(float x, float y) {

  glColor3f(0.32f, 0.32f, 0.36f);
  drawQuad(x - 4, y, x + 4, y, x + 3, y + 80, x - 3, y + 80);

  drawRect(x - 20, y + 72, x + 20, y + 74);
  drawRect(x - 15, y + 60, x + 15, y + 62);

  drawLine(x - 4, y + 30, x + 4, y + 50);
  drawLine(x + 4, y + 30, x - 4, y + 50);
  drawLine(x - 4, y + 50, x + 4, y + 70);
  drawLine(x + 4, y + 50, x - 4, y + 70);

  glColor3f(0.60f, 0.60f, 0.65f);
  drawFilledCircle(x - 18, y + 73, 2, 6);
  drawFilledCircle(x + 18, y + 73, 2, 6);
  drawFilledCircle(x - 13, y + 61, 2, 6);
  drawFilledCircle(x + 13, y + 61, 2, 6);

  glColor3f(0.45f, 0.45f, 0.48f);
  drawRect(x - 8, y, x + 8, y + 5);
}

/* ================================================================
 *  FIREWORKS (Night only)
 * ================================================================ */
#define MAX_FIREWORKS 15
int fwActive[MAX_FIREWORKS];
float fwX[MAX_FIREWORKS];
float fwY[MAX_FIREWORKS];
float fwAge[MAX_FIREWORKS];
float fwR[MAX_FIREWORKS], fwG[MAX_FIREWORKS], fwB[MAX_FIREWORKS];

void drawFireworks(void) {
  if (sceneMode != 2) return;
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  for (int k = 0; k < MAX_FIREWORKS; k++) {
     if (fwActive[k]) {
        float a = 1.0f - (fwAge[k] / 1.5f);
        if (a < 0.0f) a = 0.0f;
        float radius = fwAge[k] * 80.0f;

        for (int p = 0; p < 24; p++) {
           float angle = p * (2.0f * PI / 24.0f) + fwAge[k];
           float px = fwX[k] + cosf(angle) * radius;
           float py = fwY[k] + sinf(angle) * radius;

           glColor4f(fwR[k], fwG[k], fwB[k], a);
           glBegin(GL_LINES);
           glVertex2f(fwX[k] + cosf(angle) * (radius * 0.6f), fwY[k] + sinf(angle) * (radius * 0.6f));
           glVertex2f(px, py);
           glEnd();

           drawFilledCircle(px, py, 2.5f, 6);
        }
     }
  }
  glDisable(GL_BLEND);
}

/* ================================================================
 *  MAIN DISPLAY FUNCTION - Renders the entire scene
 * ================================================================ */
void display(void) {
  int i;
  glClear(GL_COLOR_BUFFER_BIT);

  /* ---- Layer 1: Sky and atmospheric effects ---- */
  drawSky();
  drawStars();
  drawSunMoon();
  drawClouds();
  drawFireworks();

  /* ---- Layer 2: Aircraft in sky ---- */
  drawBalloon(1150, balloonY);
  drawAirplane(planeX, planeY);

  /* ---- Layer 3: Upper desert area ---- */
  if ((sceneMode == 2))
    drawRectGradientY(0, 430, W, 465, 0.35f, 0.28f, 0.15f, 0.20f, 0.15f, 0.08f);
  else
    drawRectGradientY(0, 430, W, 465, 0.90f, 0.75f, 0.40f, 0.80f, 0.65f, 0.35f);

  /* ---- Layer 4: Pyramids (background behind river) ---- */
  drawPyramid(850, 430, 200, 150);
  drawPyramid(1000, 430, 150, 120);
  drawPyramid(720, 430, 110, 90);
  drawSteppedPyramid(50, 430, 120, 80);
  drawCrane(145, 430);
  drawTemple(190, 430);
  drawObelisk(1090, 430);

  /* ---- Layer 4.5: Egyptian Houses on the right ---- */
  drawEgyptianHouse(1160, 430, 65, 60);
  drawEgyptianHouse(1225, 430, 50, 45);
  drawEgyptianHouse(1275, 430, 55, 55);
  drawEgyptian3StoryHouse(1330, 430, 50, 40);

  /* ---- Layer 5: Nile River and boats ---- */
  drawRiver();
  drawCruiseShip(cruiseX, cruiseY);
  drawSailboat(sailX, 400);
  drawSailboat(sail2X, 395);

  /* ---- Layer 7: Desert ground with dunes ---- */
  drawGround();

  /* ---- Vertical Road & Vehicles ---- */
  drawPerspectiveRoad(cxRoad);

  if (vCar1State == 0) {
    drawCar(vCar1X, vCar1Y, 0.9f, 0.3f, 0.2f, 1);
  } else if (vCar1State == 1) {
    glPushMatrix();
    glTranslatef(vCar1X, vCar1Y, 0);
    drawVerticalCar(0.9f, 0.3f, 0.2f, 1);
    glPopMatrix();
  } else if (vCar1State == 2) {
    float f1 = (vCar1Y - 140.0f) / 305.0f;
    float scl1 = 1.0f - 0.5f * f1;
    float laneUpX = cxRoad + (25.0f - 15.0f * f1);
    glPushMatrix();
    glTranslatef(laneUpX, vCar1Y, 0);
    glScalef(scl1, scl1, 1.0f);
    drawVerticalCar(0.9f, 0.3f, 0.2f, 1);
    glPopMatrix();
  }

  if (vCar2State == 0) {
    float f2 = (vCar2Y - 140.0f) / 305.0f;
    if (f2 < 0)
      f2 = 0;
    float scl2 = 1.0f - 0.5f * f2;
    float laneDnX = cxRoad - (25.0f - 15.0f * f2);
    glPushMatrix();
    glTranslatef(laneDnX, vCar2Y, 0);
    glScalef(scl2, scl2, 1.0f);
    drawVerticalCar(0.2f, 0.8f, 0.9f, -1);
    glPopMatrix();
  } else if (vCar2State == 1) {
    float laneDnStart = cxRoad - 25.0f;
    glPushMatrix();
    glTranslatef(laneDnStart, vCar2Y, 0);
    drawVerticalCar(0.2f, 0.8f, 0.9f, -1);
    glPopMatrix();
  } else if (vCar2State == 2) {
    drawCar(vCar2X, vCar2Y - 15.0f, 0.2f, 0.8f, 0.9f, -1);
  }

  /* ---- Layer 8: Oasis and Fountain ---- */
  drawOasis(750, 280);
  drawConstructionSite(150, 240);
  drawCamel(camel1X, 220, 1.2f, -1, camelAnimTime);
  drawCamel(camel2X, 190, 0.8f, 1, camelAnimTime + PI / 2.0f);

  /* ---- Layer 9: Temple (Moved to Layer 4) ---- */

  /* ---- Layer 10: Helicopter (in front of pyramids) ---- */
  drawHelicopter(heliX, heliY);

  /* ---- Layer 11: Construction Crane (Moved to Layer 4) ---- */

  /* ---- Layer 12: Wind Turbine (moved to river bank) ---- */
  drawWindTurbine(950, 365);

  /* ---- Layer 13: Power Towers + wires + New structures ---- */
  drawPowerTower(1050, 260);
  drawPowerTower(1120, 260);
  drawModernBuildings(1175, 290);
  drawRuinedTemples(1150, 240);

  glColor3f(0.20f, 0.20f, 0.22f);
  glLineWidth(1.2f);

  drawLine(1068, 333, 1102, 333);
  drawLine(1063, 321, 1107, 321);

  drawLine(1060, 365, 1032, 333);

  drawLine(1145, 362, 1138, 333);

  drawLine(1037, 321, 1020, 217);

  drawLine(1133, 321, 1170, 217);
  glLineWidth(1.0f);

  /* ---- Layer 14: Solar Panels (moved to river bank) ---- */
  drawSolarPanel(1050, 365);
  drawSolarPanel(1120, 362);

  /* ---- Layer 15: Road ---- */
  drawRoad();

  /* ---- Layer 16: Vehicles on road ---- */
  drawCar(carRedX, 60, 0.85f, 0.15f, 0.10f, -1);
  drawCar(carBlueX, 105, 0.15f, 0.30f, 0.85f, 1);
  drawBus(busX, 58);
  drawTruck(truckX, 100);
  drawMotorcycle(bikeX, 115);

  /* ---- Layer 17: Traffic lights ---- */
  drawTrafficLight(630, 140);
  drawTrafficLight(760, 140);

  /* ---- Layer 18: Street lamps ---- */
  for (i = 0; i < NUM_LAMPS; i++) {
    drawStreetLamp(lampPosX[i], 155);
  }

  /* ---- Layer 19: Billboards ---- */
  drawBillboard(440, 155, "");
  drawBillboard(800, 155, "");

  /* ---- Layer 21: Normal Trees ---- */
  for (i = 0; i < NUM_PALMS; i++) {
    float sw = (i % 2 == 0) ? palmSw : -palmSw;
    drawTree(palmTreeX[i], palmTreeY[i], sw);
  }

  /* ---- Layer 22: Cacti ---- */
  drawCactus(1100, 155);
  drawCactus(1330, 160);
  drawCactus(50, 365);

  glutSwapBuffers();
}

/* ================================================================
 *  TIMER CALLBACK - Updates all animations every 16ms (~60 FPS)
 * ================================================================ */
void timer(int value) {
  int i;

  if (!paused) {
    autoModeTimer++;
    if (autoModeTimer >= 1250) {
      sceneMode = (sceneMode + 1) % 3;
      autoModeTimer = 0;
    }

    int stopRed = (trafSt == 0);

    if (!(stopRed && carRedX > 755 && carRedX < 770)) {
      carRedX -= vSpeed;
      if (carRedX < -80)
        carRedX = W + 50;
    }

    if (!(stopRed && carBlueX > 575 && carBlueX < 595)) {
      carBlueX += vSpeed * 1.2f;
      if (carBlueX > W + 50)
        carBlueX = -80;
    }

    if (!(stopRed && busX > 755 && busX < 770)) {
      busX -= vSpeed * 0.8f;
      if (busX < -130)
        busX = W + 60;
    }

    if (!(stopRed && truckX > 535 && truckX < 560)) {
      truckX += vSpeed * 0.9f;
      if (truckX > W + 60)
        truckX = -130;
    }

    if (!(stopRed && bikeX > 600 && bikeX < 625)) {
      bikeX += vSpeed * 1.5f;
      if (bikeX > W + 40)
        bikeX = -40;
    }

    float laneUpStart = cxRoad + 25.0f;
    if (vCar1State == 0) {
      vCar1X += vSpeed * 1.2f;
      if (vCar1X >= laneUpStart) {
        vCar1X = laneUpStart;
        vCar1State = 1;
      }
    } else if (vCar1State == 1) {
      vCar1Y += vSpeed * 0.5f;
      if (vCar1Y >= 140.0f) {
        vCar1State = 2;
      }
    } else if (vCar1State == 2) {
      bool vWaitC1 = (vCar1Y > 185 && vCar1Y < 205 && camel1X > 370 && camel1X < 530);
      bool vWaitC2 = (vCar1Y > 155 && vCar1Y < 175 && camel2X > 370 && camel2X < 530);
      if (!vWaitC1 && !vWaitC2) {
        vCar1Y += vSpeed * 0.5f;
      }
      if (vCar1Y > 450.0f) {
        vCar1State = 0;
        vCar1X = -100.0f;
        vCar1Y = 105.0f;
      }
    }

    float laneDnStart = cxRoad - 25.0f;
    if (vCar2State == 0) {
      bool vWaitC1 = (vCar2Y > 230 && vCar2Y < 250 && camel1X > 360 && camel1X < 540);
      bool vWaitC2 = (vCar2Y > 200 && vCar2Y < 220 && camel2X > 360 && camel2X < 540);
      if (!vWaitC1 && !vWaitC2) {
        vCar2Y -= vSpeed * 0.4f;
      }
      if (vCar2Y <= 140.0f) {
        vCar2State = 1;
      }
    } else if (vCar2State == 1) {
      vCar2Y -= vSpeed * 0.4f;
      if (vCar2Y <= 75.0f) {
        vCar2Y = 75.0f;
        vCar2State = 2;
        vCar2X = laneDnStart;
      }
    } else if (vCar2State == 2) {
      vCar2X -= vSpeed * 1.5f;
      if (vCar2X < -150.0f) {
        vCar2State = 0;
        vCar2Y = 450.0f;
      }
    }

    camelAnimTime += 0.05f;
    camel1X -= vSpeed * 0.2f;
    if (camel1X < -100)
      camel1X = W + 100;
    camel2X += vSpeed * 0.15f;
    if (camel2X > W + 100)
      camel2X = -100;

    heliY += (float)heliDir * 0.35f;
    heliX += 2.0f;
    if (heliX > W + 200)
      heliX = -200.0f;
    if (heliY > 705)
      heliDir = -1;
    if (heliY < 655)
      heliDir = 1;

    balloonY += (float)ballDir * 0.22f;
    if (balloonY > 675)
      ballDir = -1;
    if (balloonY < 625)
      ballDir = 1;

    planeX -= vSpeed * 1.8f;
    if (planeX < -200.0f)
      planeX = W + 100.0f;

    sailX += 0.9f;
    if (sailX > W + 60)
      sailX = -60;
    sail2X += 0.6f;
    if (sail2X > W + 60)
      sail2X = -60;

    if (cruiseState == 0) {
      cruiseX -= vSpeed * 0.7f;
      if (cruiseX <= 850.0f) { // Stop in front of main pyramid
        cruiseState = 1;
        cruiseWaitTimer = 0;
      }
    } else if (cruiseState == 1) {
      cruiseWaitTimer++;
      if (cruiseWaitTimer >= 300) { // 5 sec wait at ~60 FPS
        cruiseState = 2;
      }
    } else if (cruiseState == 2) {
      cruiseX -= vSpeed * 0.7f;
      if (cruiseX < -200.0f) {
        cruiseX = W + 200.0f;
        cruiseState = 0;
      }
    }

    cloudOff += 0.55f;

    craneAng += (float)craneDir * 0.35f;
    if (craneAng > 35)
      craneDir = -1;
    if (craneAng < -15)
      craneDir = 1;

    turbAng += 2.5f;
    if (turbAng >= 360)
      turbAng -= 360;

    trafTm++;
    if (trafTm > 90) {
      trafSt = (trafSt + 1) % 3;
      trafTm = 0;
    }

    palmSw = 0.0f;

    if (palmSw > 0.18f)
      palmDir = -1;
    if (palmSw < -0.18f)
      palmDir = 1;

    sunG += 0.032f;

    for (i = 0; i < MAX_FOUNT; i++) {
      fntY[i] += fntVY[i];
      fntX[i] += fntVX[i];
      fntVY[i] -= 0.05f;
      if (fntY[i] < 0 || fntY[i] > 28) {
        fntY[i] = 0;
        fntX[i] = (rand() % 18) - 9;
        fntVY[i] = 0.8f + (rand() % 25) / 10.0f;
        fntVX[i] = ((rand() % 10) - 5) / 10.0f;
      }
    }

    for (i = 0; i < MAX_FIREWORKS; i++) {
        if (fwActive[i]) {
            fwAge[i] += 0.025f;
            fwY[i] -= 0.6f;
            if (fwAge[i] > 1.5f) fwActive[i] = 0;
        }
    }
  }

  glutPostRedisplay();
  glutTimerFunc(16, timer, 0);
}

/* ================================================================
 *  KEYBOARD CALLBACK - Handle key presses
 * ================================================================ */
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 'd':
  case 'D':
    sceneMode = (sceneMode + 1) % 3;
    break;

  case 'r':
  case 'R':
    if (sceneMode == 2) {
       int toSp = 3 + (rand() % 2);
       int spawned = 0;
       for (int k = 0; k < MAX_FIREWORKS && spawned < toSp; k++) {
          if (!fwActive[k]) {
             fwActive[k] = 1;
             fwX[k] = 200.0f + (rand() % 1000);
             fwY[k] = 450.0f + (rand() % 280);
             fwAge[k] = 0.0f;
             int c = rand() % 6;
             fwR[k] = (c==0||c==3||c==4) ? 1.0f : ((rand() % 100) / 100.0f);
             fwG[k] = (c==1||c==3||c==5) ? 1.0f : ((rand() % 100) / 100.0f);
             fwB[k] = (c==2||c==4||c==5) ? 1.0f : ((rand() % 100) / 100.0f);
             spawned++;
          }
       }
    }
    break;

  case 'p':
  case 'P':
    paused = !paused;
    break;
  case '+':
  case '=':
    vSpeed += 0.5f;
    if (vSpeed > 8.0f)
      vSpeed = 8.0f;
    break;
  case '-':
  case '_':
    vSpeed -= 0.5f;
    if (vSpeed < 0.5f)
      vSpeed = 0.5f;
    break;
  case 27:
    exit(0);
    break;
  }
  glutPostRedisplay();
}

/* ================================================================
 *  INITIALIZATION - Set up OpenGL state and particle data
 * ================================================================ */
void init(void) {
  int i;

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, W, 0, H);
  glMatrixMode(GL_MODELVIEW);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  srand((unsigned int)42);

  for (i = 0; i < MAX_FOUNT; i++) {
    fntX[i] = (rand() % 18) - 9;
    fntY[i] = rand() % 25;
    fntVY[i] = 0.8f + (rand() % 25) / 10.0f;
    fntVX[i] = ((rand() % 10) - 5) / 10.0f;
  }

  for (i = 0; i < MAX_STARS; i++) {
    starX[i] = rand() % W;
    starY[i] = 380 + rand() % (H - 380);
    starB[i] = 0.25f + (rand() % 75) / 100.0f;
    starSpd[i] = 1.5f + (rand() % 50) / 10.0f;
  }
}

/* ================================================================
 *  MAIN ENTRY POINT
 * ================================================================ */
int main(int argc, char **argv) {

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(W, H);
  glutInitWindowPosition(50, 30);
  glutCreateWindow("Complex Egyptian Desert Scene - OpenGL 2D Project");

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutTimerFunc(16, timer, 0);

  printf("==============================================\n");
  printf(" Complex Egyptian Desert Scene - OpenGL 2D\n");
  printf("==============================================\n");
  printf(" CONTROLS:\n");
  printf("   D - Toggle Day/Night mode/sunset\n");
  printf("   P - Pause/Resume animation\n");
  printf("   + - Increase vehicle speed\n");
  printf("   - - Decrease vehicle speed\n");
  printf("   ESC - Exit\n");
  printf("==============================================\n");

  glutMainLoop();

  return 0;
}

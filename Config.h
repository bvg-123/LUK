#pragma once

/**************************************************
 *
 * Параметры системы
 *
 **************************************************/

/** Ускорение свободного падения (кг -> Н) */
const float G = 10;

/** PI */
const float PI = 3.1415926;

// image-comment: [.codeimages/img-config.png]

/** Расстояние между осями дверей по-горизонтали, м */
const float AXES_HORIZONTAL_OFFSET = 0.15;

/** Расстояние между осями дверей по вертикали, м */
const float AXES_VERTICAL_OFFSET = 0.27;

/** Ширина верхней двери, м */
const float TOP_DOOR_LENGTH = 1.25;

/** Масса верхней двери, кг */
const float TOP_DOOR_WEIGHT = 76;

/** Ширина нижней двери, м */
const float BOTTOM_DOOR_LENTH = 1;

/** Масса нижней двери, кг */
const float BOTTOM_DOOR_WEIGHT = 30;

/** Длина веревки, соединяющей верхнюю и нижнюю двери, м */
const float CORD_LENGTH = 0.6;

// Сила действия одного амортизатора, Н
const float AMMO_FORCE = 1200;

// Максимальная длина амортизатора, м
const float AMMO_MAX_LENGTH = 0.46;

// Длина амортизатора в сжатом состоянии при полностью закрытой верхней двери, м
const float AMMO_MIN_LENGTH = 0.3;//AMMO_MAX_LENGTH - 0.19;

// Количество амортизаторов
const int AMMO_COUNT = 3;

// Расстояние по горизонтали от неподвижной оси амортизатора до оси верхней двери, м
const float AMMO_AXIS_HORIZONTAL_OFFSET = 0.025;

// Расстояние по вертикали от неподвижной оси амортизатора до оси верхней двери, м
const float AMMO_AXIS_VERTICAL_OFFSET = 0.165;

// Смещение крепления штока амортизатора по вертикали относительно центра масс верхней двери в закрытом положении.
const float AMMO_TOP_DOOR_OFFSET = 0.024;
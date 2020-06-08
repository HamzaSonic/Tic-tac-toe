#include <iostream>
#include <string>
#include <vector>
#include <glut.h>

using namespace std;

// class Color
class Color
{
public:
	Color(double r, double g, double b)
		: red(r), green(g), blue(b)
	{}
	double red, green, blue;
};
void DrawCircle(float cx, float cy, float r, int num_segments, Color color)
{
	glColor3f(color.red, color.green, color.blue);
	glBegin(GL_LINE_LOOP);
	for (int ii = 0; ii < num_segments; ii++)
	{
		float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);

		float x = r * cosf(theta);
		float y = r * sinf(theta);

		glVertex2f(x + cx, y + cy);

	}
	glEnd();
}
Color clrPlayer1(0 / 255.0, 51 / 255.0, 102 / 255.0);
Color clrPlayer2(204.0 / 255.0, 0.0 / 255.0, 102.0 / 255.0);

// class Point2D
class Point2D
{
public:
	Point2D(float a = 0.0, float b = 0.0)
		: x(a), y(b)
	{}

	float x, y;
};
void DrawCross(Color color, Point2D pntCenter, int length)
{
	glColor3f(color.red, color.green, color.blue);
	glLineWidth(10);
	glBegin(GL_LINES);
	glVertex2f(pntCenter.x - length / 2.0, pntCenter.y - length / 2.0);
	glVertex2f(pntCenter.x + length / 2.0, pntCenter.y + length / 2.0);
	glVertex2f(pntCenter.x - length / 2.0, pntCenter.y + length / 2.0);
	glVertex2f(pntCenter.x + length / 2.0, pntCenter.y - length / 2.0);
	glEnd();
}

// class Rectangle
class Rectangle
{
public:
	Rectangle()
		: pntBottomLeft(Point2D(0.0, 0.0)), width(0.0), height(0.0)
	{}
	Rectangle(Point2D p, double w, double h)
		: pntBottomLeft(p), width(w), height(h)
	{}
	void Draw(const Color& color) const
	{
		glColor3f(color.red, color.green, color.blue);
		glLineWidth(10);
		glBegin(GL_LINE_LOOP);
		glVertex2f(pntBottomLeft.x, pntBottomLeft.y);
		glVertex2f(pntBottomLeft.x + width, pntBottomLeft.y);
		glVertex2f(pntBottomLeft.x + width, pntBottomLeft.y + height);
		glVertex2f(pntBottomLeft.x, pntBottomLeft.y + height);
		glEnd();
	}
	void DrawFilled(const Color& color) const
	{
		glColor3f(color.red, color.green, color.blue);
		glBegin(GL_QUADS);
		glVertex2f(pntBottomLeft.x, pntBottomLeft.y);
		glVertex2f(pntBottomLeft.x + width, pntBottomLeft.y);
		glVertex2f(pntBottomLeft.x + width, pntBottomLeft.y + height);
		glVertex2f(pntBottomLeft.x, pntBottomLeft.y + height);
		glEnd();
	}
	Point2D pntBottomLeft; // bottom left point
	double width, height;
};

// class Rectangle
class Cell
{
public:

	Cell(Rectangle rec, int r, int c)
		:rec(rec), row(r), col(c)
	{}
	bool IsInside(Point2D pnt) const
	{
		if (pnt.x >= rec.pntBottomLeft.x && pnt.x <= rec.pntBottomLeft.x + rec.width &&
			pnt.y >= rec.pntBottomLeft.y && pnt.y <= rec.pntBottomLeft.y + rec.height)
			return true;

		return false;
	}
	Point2D GetCenter() const
	{
		return Point2D(rec.pntBottomLeft.x + rec.width / 2.0, rec.pntBottomLeft.y + rec.height / 2.0);
	}
	int GetRow() const
	{
		return row;
	}
	int GetColumn() const
	{
		return col;
	}
	void Draw(const Color& color) const
	{
		rec.DrawFilled(color);
		rec.Draw(Color(0, 0, 0));
		if (!clicked) {
			if (!whoClicked) {
				DrawCross(clrPlayer2, GetCenter(), 60);
			}
			else {
				DrawCircle(GetCenter().x, GetCenter().y, 40, 360, clrPlayer1);
			}
		}
	}
	bool clicked;
	bool whoClicked;
	Rectangle rec;
	int row, col;		// row and column indices of the 
};
// class Circle
/*class Circle
{
public:
	Circle(Point2D pnt, float r)
	: pntCenter(pnt), radius(r)
	{}
	void DrawFilled(const Color& color) const
	{
		glColor3f(color.red, color.green, color.blue);
		glLineWidth(10);
		const float DEG2RAD = 3.14159 / 180;
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(pntCenter.x,
			pntCenter.y);
		for (int i = 0; i <= 360; i++) {
			float degInRad = i * DEG2RAD;

			glVertex2f(pntCenter.x + cos(degInRad) * radius,
				pntCenter.y + sin(degInRad) * radius);
		}
		glEnd();
	}
	void Draw(const Color& color) const
	{
		glColor3f(color.red, color.green, color.blue);
		glPointSize(5);

		Point2D pntCircle;

		int p = 1 - radius;

		pntCircle.x = 0;
		pntCircle.y = radius;

		// plot the initial point in each circle quadrant
		circle_plot_points(pntCircle);

		// calculate next point and plot in each quadrant
		while (pntCircle.x < pntCircle.y) {
			++pntCircle.x;
			if (p < 0)
				p += 2 * pntCircle.x + 1;
			else
			{
				--pntCircle.y;
				p += 2 * (pntCircle.x - pntCircle.y) + 1;
			}
			circle_plot_points(pntCircle);
		}
	}

	Point2D pntCenter;
	float radius;

private:
	void circle_plot_points(Point2D pntScreen) const
	{
		setPixel(pntCenter.x + pntScreen.x, pntCenter.y + pntScreen.y);
		setPixel(pntCenter.x - pntScreen.x, pntCenter.y + pntScreen.y);
		setPixel(pntCenter.x + pntScreen.x, pntCenter.y - pntScreen.y);
		setPixel(pntCenter.x - pntScreen.x, pntCenter.y - pntScreen.y);
		setPixel(pntCenter.x + pntScreen.y, pntCenter.y + pntScreen.x);
		setPixel(pntCenter.x - pntScreen.y, pntCenter.y + pntScreen.x);
		setPixel(pntCenter.x + pntScreen.y, pntCenter.y - pntScreen.x);
		setPixel(pntCenter.x - pntScreen.y, pntCenter.y - pntScreen.x);
	}
	void setPixel(int x, int y) const
	{
		glBegin(GL_POINTS);
			glVertex2i(x, y);
		glEnd();
	}
};*/



double z = 0;
bool w = 1;
bool sleep = 0;
void sleepy()
{
	if (z > 4)
		w = 0;
	if (z < -4)
		w = 1;
	if (w && 1)
		z += 0.01;
	else
		z -= 0.01;
	glBegin(GL_LINES);
	glVertex2d(450 + 89 + z, 290);
	glVertex2d(450 + 79 + z, 290);
	glVertex2d(450 + 79 + z, 290);
	glVertex2d(450 + 89 + z, 303);
	glVertex2d(450 + 89 + z, 303);
	glVertex2d(450 + 79 + z, 304);
	glEnd();

	glBegin(GL_LINES);
	glVertex2d(450 + 76 + z, 331);
	glVertex2d(450 + 61 + z, 331);
	glVertex2d(450 + 61 + z, 331);
	glVertex2d(450 + 76 + z, 349);
	glVertex2d(450 + 76 + z, 349);
	glVertex2d(450 + 61 + z, 349);
	glEnd();

	glBegin(GL_LINES);
	glVertex2d(450 + 55 + z, 370);
	glVertex2d(450 + 36 + z, 369);
	glVertex2d(450 + 36 + z, 369);
	glVertex2d(450 + 55 + z, 394);
	glVertex2d(450 + 55 + z, 394);
	glVertex2d(450 + 36 + z, 394);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex2d(0.35 * 113 + 450, 0.35 * 155 + 210);
	glVertex2d(0.35 * 132 + 450, 0.35 * 165 + 210);
	glVertex2d(0.35 * 132 + 450, 0.35 * 185 + 210);
	glVertex2d(0.35 * 115 + 450, 0.35 * 195 + 210);
	glVertex2d(0.35 * 97 + 450, 0.35 * 185 + 210);
	glVertex2d(0.35 * 97 + 450, 0.35 * 165 + 210);
	glVertex2d(0.35 * 113 + 450, 0.35 * 155 + 210);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex2d(0.35 * 116 + 450, 0.35 * 156 + 210);
	glVertex2d(0.35 * 126 + 450, 0.35 * 162 + 210);
	glVertex2d(0.35 * 126 + 450, 0.35 * 175 + 210);
	glVertex2d(0.35 * 116 + 450, 0.35 * 180 + 210);
	glVertex2d(0.35 * 105 + 450, 0.35 * 175 + 210);
	glVertex2d(0.35 * 105 + 450, 0.35 * 162 + 210);
	glVertex2d(0.35 * 116 + 450, 0.35 * 156 + 210);
	glEnd();
}



std::vector<Cell> vecCells;

const int player_1 = 0, player_2 = 1;
const int numPlayers = 2;

int activePlayer = player_1;
int winner = -1;
bool gameOver = false;
bool tie = false;
bool turn = false;
// players colors

// rectangle backround color
Color clrRecBackGrd(204.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);

string playerNames[numPlayers];

int plays[3][3] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };	// track the moves of each play

// global variables
int width, height;
bool verg = 0;
void makeVertex() {
	if (plays[0][0] == plays[0][1] && plays[0][2] == plays[0][0] && plays[0][0] != -1)
	{
		float x1 = vecCells[0].GetCenter().x, x2 = vecCells[2].GetCenter().x, y1 = vecCells[0].GetCenter().y, y2 = vecCells[2].GetCenter().y;
		glColor3f(1.0, 0.0, 0.0);
		glLineWidth(10);
		glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
		glEnd();
	}
	else {
		if (plays[1][0] == plays[1][1] && plays[1][2] == plays[1][0] && plays[1][0] != -1)
		{
			float x1 = vecCells[3].GetCenter().x, x2 = vecCells[5].GetCenter().x, y1 = vecCells[3].GetCenter().y, y2 = vecCells[5].GetCenter().y;
			glColor3f(1.0, 0.0, 0.0);
			glLineWidth(10);
			glBegin(GL_LINES);
			glVertex2f(x1, y1);
			glVertex2f(x2, y2);
			glEnd();
		}
		else {
			if (plays[2][0] == plays[2][1] && plays[2][2] == plays[2][0] && plays[2][0] != -1)
			{
				float x1 = vecCells[6].GetCenter().x, x2 = vecCells[8].GetCenter().x, y1 = vecCells[6].GetCenter().y, y2 = vecCells[8].GetCenter().y;
				glColor3f(1.0, 0.0, 0.0);
				glLineWidth(10);
				glBegin(GL_LINES);
				glVertex2f(x1, y1);
				glVertex2f(x2, y2);
				glEnd();
			}
			else {
				if (plays[0][0] == plays[1][0] && plays[2][0] == plays[0][0] && plays[0][0] != -1)
				{
					float x1 = vecCells[0].GetCenter().x, x2 = vecCells[6].GetCenter().x, y1 = vecCells[0].GetCenter().y, y2 = vecCells[6].GetCenter().y;
					glColor3f(1.0, 0.0, 0.0);
					glLineWidth(10);
					glBegin(GL_LINES);
					glVertex2f(x1, y1);
					glVertex2f(x2, y2);
					glEnd();
				}
				else {
					if (plays[0][1] == plays[1][1] && plays[2][1] == plays[0][1] && plays[0][1] != -1)
					{
						float x1 = vecCells[1].GetCenter().x, x2 = vecCells[7].GetCenter().x, y1 = vecCells[1].GetCenter().y, y2 = vecCells[7].GetCenter().y;
						glColor3f(1.0, 0.0, 0.0);
						glLineWidth(10);
						glBegin(GL_LINES);
						glVertex2f(x1, y1);
						glVertex2f(x2, y2);
						glEnd();
					}
					else {
						if (plays[0][2] == plays[1][2] && plays[2][2] == plays[0][2] && plays[0][2] != -1)
						{
							float x1 = vecCells[2].GetCenter().x, x2 = vecCells[8].GetCenter().x, y1 = vecCells[2].GetCenter().y, y2 = vecCells[8].GetCenter().y;
							glColor3f(1.0, 0.0, 0.0);
							glLineWidth(10);
							glBegin(GL_LINES);
							glVertex2f(x1, y1);
							glVertex2f(x2, y2);
							glEnd();
						}
						else {
							if (plays[0][0] == plays[1][1] && plays[2][2] == plays[0][0])
							{
								float x1 = vecCells[0].GetCenter().x, x2 = vecCells[8].GetCenter().x, y1 = vecCells[0].GetCenter().y, y2 = vecCells[8].GetCenter().y;
								glColor3f(1.0, 0.0, 0.0);
								glLineWidth(10);
								glBegin(GL_LINES);
								glVertex2f(x1, y1);
								glVertex2f(x2, y2);
								glEnd();
							}
							else {
								if (plays[0][2] == plays[1][1] && plays[2][0] == plays[0][2])
								{
									float x1 = vecCells[2].GetCenter().x, x2 = vecCells[6].GetCenter().x, y1 = vecCells[2].GetCenter().y, y2 = vecCells[6].GetCenter().y;
									glColor3f(1.0, 0.0, 0.0);
									glLineWidth(10);
									glBegin(GL_LINES);
									glVertex2f(x1, y1);
									glVertex2f(x2, y2);
									glEnd();
								}
							}
						}
					}
				}
			}
		}
	}
}


void renderBitmapString(float x, float y, float z, void* font, const char* string)
{
	const char *c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++)
		glutBitmapCharacter(font, *c);
}

void DrawGrid()
{
	if (!vecCells.empty())
		vecCells.clear();

	// drawing area is 50% of available area
	float drawingWidth = 0.5 * width;
	float drawingHeight = 0.5 * height;

	float startX = 0.25 * width;
	float startY = 0.25 * height;

	float cellWidth = drawingWidth / 3;
	float cellHeight = drawingHeight / 3;


	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j) {
			Cell cell(Rectangle(Point2D(startX + j * cellWidth, startY + i * cellHeight), cellWidth, cellHeight), i, j);
			vecCells.push_back(cell);
		}
}

void DrawPlayer(const Rectangle& rec, int player)
{
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(2);
	//int x=8,y=10;
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(24, 210 + 11);
	glVertex2f(81, 210 + 11);
	glVertex2f(81, 210 + 44 * 3 - 14);
	glVertex2f(24, 210 + 44 * 3 - 14);


	glVertex2f(25, 210 + 44 * 3 - 14);
	glVertex2f(35, 210 + 44 * 3 - 14);
	glVertex2f(35, 210 + 44 * 3 + 6);
	glVertex2f(25, 210 + 44 * 3 + 6);

	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.647059, 0.164706, 0.164706);
	glVertex2f(81, 210 + 38);
	glVertex2f(81 + 20, 210 + 38 + 15);
	glVertex2f(81, 210 + 38 + 15);

	glColor3f(1.0, 1.0, 0.0);
	glVertex2f(81 + 20, 210 + 38 + 15);
	glVertex2f(81 + 20, 210 + 38 + 21);
	glVertex2f(81, 210 + 38 + 21);
	glVertex2f(81, 210 + 38 + 15);
	glVertex2f(81 + 14, 210 + 38 + 15);
	glVertex2f(81 + 14, 210 + 38 + 45);
	glVertex2f(81, 210 + 38 + 45);
	glVertex2f(81, 210 + 38 + 15);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(81 + 14, 210 + 38 + 45);
	glVertex2f(81 + 14, 210 + 38 + 30);
	glVertex2f(81 + 19, 210 + 38 + 30);
	glVertex2f(81 + 19, 210 + 38 + 45);
	glEnd();


	glBegin(GL_POLYGON);
	glVertex2f(81, 210 + 38 + 63);
	glVertex2f(81 + 10, 210 + 38 + 63);
	glVertex2f(81 + 10, 210 + 38 + 63 + 15);
	glVertex2f(81, 210 + 38 + 63 + 15);

	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(81 + 10, 210 + 38 + 63 + 15);
	glVertex2f(81 + 10, 210 + 38 + 63 + 3);
	glVertex2f(81 + 21, 210 + 38 + 63 + 16);

	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(29, 17 + 210);
	glVertex2f(35, 17 + 210);
	glVertex2f(28, 10 + 210);
	glVertex2f(28, 16 + 210);
	glVertex2f(28, 10 + 210);
	glVertex2f(46, 10 + 210);
	glVertex2f(47, 11 + 210);
	glVertex2f(47, 17 + 210);
	glVertex2f(47, 17 + 210);
	glVertex2f(59, 17 + 210);
	glVertex2f(59, 17 + 210);
	glVertex2f(59, 11 + 210);
	glVertex2f(60, 10 + 210);
	glVertex2f(78, 10 + 210);
	glVertex2f(78, 10 + 210);
	glVertex2f(78, 16 + 210);
	glVertex2f(77, 17 + 210);
	glVertex2f(71, 17 + 210);

	glVertex2f(78, 18 + 210);
	glVertex2f(78, 21 + 210);
	glVertex2f(79, 21 + 210);
	glVertex2f(79, 24 + 210);
	glVertex2f(80, 24 + 210);
	glVertex2f(80, 30 + 210);
	glVertex2f(81, 30 + 210);
	glVertex2f(81, 51 + 210);
	glVertex2f(80, 51 + 210);
	glVertex2f(80, 57 + 210);
	glVertex2f(79, 57 + 210);
	glVertex2f(79, 63 + 210);

	glVertex2f(80, 63 + 210);
	glVertex2f(80, 66 + 210);
	glVertex2f(81, 66 + 210);
	glVertex2f(81, 85 + 210);

	glVertex2f(80, 85 + 210);
	glVertex2f(80, 94 + 210);

	glVertex2f(79, 94 + 210);
	glVertex2f(79, 100 + 210);

	glVertex2f(77, 100 + 210);
	glVertex2f(77, 103 + 210);
	glVertex2f(80, 100 + 210);
	glVertex2f(86, 100 + 210);
	int x = 87, y = 100;
	for (int i = 1; i <= 18; ++i) {
		glVertex2f(x, y + 210);
		glVertex2f(x, y + 2 + 210);
		x += 1;
		y += 1;
	}
	x = 88; y = 102;
	for (int i = 1; i <= 15; ++i) {
		glVertex2f(x, y + 210);
		glVertex2f(x, y + 2 + 210);
		x += 1;
		y += 1;
	}
	y += 2;
	x += 1;
	glVertex2f(x, y + 210);
	glVertex2f(x - 12, y + 210);

	glVertex2f(104, 117 + 210);
	glVertex2f(83, 117 + 210);
	//enzal 1 o orsom 5a6 2
	glVertex2f(83, 116 + 210);
	glVertex2f(78, 116 + 210);
	glVertex2f(78, 115 + 210);
	glVertex2f(72, 115 + 210);
	//ghala6
	glVertex2f(72, 114 + 210);
	glVertex2f(69, 114 + 210);
	glVertex2f(69, 113 + 210);
	glVertex2f(66, 113 + 210);
	glVertex2f(66, 114 + 210);
	glVertex2f(42, 114 + 210);
	glVertex2f(42, 113 + 210);
	glVertex2f(39, 113 + 210);


	glVertex2f(38, 114 + 210);
	glVertex2f(38, 120 + 210);
	glVertex2f(37, 120 + 210);
	glVertex2f(37, 129 + 210);
	glVertex2f(36, 129 + 210);
	glVertex2f(36, 135 + 210);
	glVertex2f(35, 135 + 210);
	glVertex2f(35, 141 + 210);
	glVertex2f(34, 138 + 210);
	glVertex2f(34, 147 + 210);
	glVertex2f(33, 138 + 210);
	glVertex2f(33, 147 + 210);
	glVertex2f(32, 138 + 210);
	glVertex2f(32, 147 + 210);
	glVertex2f(31, 138 + 210);
	glVertex2f(31, 150 + 210);
	glVertex2f(30, 138 + 210);
	glVertex2f(30, 150 + 210);
	glVertex2f(29, 138 + 210);
	glVertex2f(29, 150 + 210);
	glVertex2f(28, 147 + 210);
	glVertex2f(28, 138 + 210);
	glVertex2f(27, 147 + 210);
	glVertex2f(27, 138 + 210);
	glVertex2f(26, 147 + 210);
	glVertex2f(26, 117 + 210);
	glVertex2f(27, 117 + 210);
	glVertex2f(27, 108 + 210);
	glVertex2f(28, 108 + 210);
	glVertex2f(28, 102 + 210);
	glVertex2f(29, 102 + 210);
	glVertex2f(29, 96 + 210);


	//mirror
	glVertex2f(35, 18 + 210);
	glVertex2f(29, 21 + 210);
	glVertex2f(29, 21 + 210);
	glVertex2f(29, 24 + 210);
	glVertex2f(27, 24 + 210);
	glVertex2f(27, 30 + 210);
	glVertex2f(25, 30 + 210);
	glVertex2f(25, 51 + 210);
	glVertex2f(26, 51 + 210);
	glVertex2f(26, 57 + 210);
	glVertex2f(27, 57 + 210);
	glVertex2f(27, 63 + 210);

	glVertex2f(28, 63 + 210);
	glVertex2f(28, 66 + 210);
	glVertex2f(27, 66 + 210);
	glVertex2f(27, 85 + 210);

	glVertex2f(28, 85 + 210);
	glVertex2f(28, 94 + 210);

	glVertex2f(29, 94 + 210);
	glVertex2f(29, 100 + 210);

	//alwan
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(30, 76 + 210);
	glVertex2f(36, 76 + 210);
	glVertex2f(30, 75 + 210);
	glVertex2f(36, 75 + 210);
	glVertex2f(27, 77 + 210);
	glVertex2f(39, 77 + 210);
	glVertex2f(27, 78 + 210);
	glVertex2f(39, 78 + 210);
	glVertex2f(27, 79 + 210);
	glVertex2f(39, 79 + 210);
	glVertex2f(27, 80 + 210);
	glVertex2f(39, 80 + 210);
	glVertex2f(30, 81 + 210);
	glVertex2f(36, 81 + 210);
	glVertex2f(30, 82 + 210);
	glVertex2f(36, 82 + 210);


	//mirror
	glVertex2f(30 + 42, 76 + 210);
	glVertex2f(36 + 42, 76 + 210);
	glVertex2f(30 + 42, 75 + 210);
	glVertex2f(36 + 42, 75 + 210);
	glVertex2f(27 + 42, 77 + 210);
	glVertex2f(39 + 42, 77 + 210);
	glVertex2f(27 + 42, 78 + 210);
	glVertex2f(39 + 42, 78 + 210);
	glVertex2f(27 + 42, 79 + 210);
	glVertex2f(39 + 42, 79 + 210);
	glVertex2f(27 + 42, 80 + 210);
	glVertex2f(39 + 42, 80 + 210);
	glVertex2f(30 + 42, 81 + 210);
	glVertex2f(36 + 42, 81 + 210);
	glVertex2f(30 + 42, 82 + 210);
	glVertex2f(36 + 42, 82 + 210);

	//eyes
	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(30 + 10, 77 + 13 + 210);
	glVertex2f(36 + 10, 77 + 13 + 210);
	glVertex2f(30 + 10, 76 + 13 + 210);
	glVertex2f(36 + 10, 76 + 13 + 210);
	glVertex2f(27 + 10, 78 + 13 + 210);
	glVertex2f(39 + 10, 78 + 13 + 210);
	glVertex2f(27 + 10, 79 + 13 + 210);
	glVertex2f(39 + 10, 79 + 13 + 210);
	glVertex2f(27 + 10, 80 + 13 + 210);
	glVertex2f(39 + 10, 80 + 13 + 210);
	glVertex2f(27 + 10, 81 + 13 + 210);
	glVertex2f(39 + 10, 81 + 13 + 210);
	glVertex2f(30 + 10, 82 + 13 + 210);
	glVertex2f(36 + 10, 82 + 13 + 210);
	glVertex2f(30 + 10, 83 + 13 + 210);
	glVertex2f(36 + 10, 83 + 13 + 210);

	glVertex2f(30 + 32, 77 + 13 + 210);
	glVertex2f(36 + 32, 77 + 13 + 210);
	glVertex2f(30 + 32, 76 + 13 + 210);
	glVertex2f(36 + 32, 76 + 13 + 210);
	glVertex2f(27 + 32, 78 + 13 + 210);
	glVertex2f(39 + 32, 78 + 13 + 210);
	glVertex2f(27 + 32, 79 + 13 + 210);
	glVertex2f(39 + 32, 79 + 13 + 210);
	glVertex2f(27 + 32, 80 + 13 + 210);
	glVertex2f(39 + 32, 80 + 13 + 210);
	glVertex2f(27 + 32, 81 + 13 + 210);
	glVertex2f(39 + 32, 81 + 13 + 210);
	glVertex2f(30 + 32, 82 + 13 + 210);
	glVertex2f(36 + 32, 82 + 13 + 210);
	glVertex2f(30 + 32, 83 + 13 + 210);
	glVertex2f(36 + 32, 83 + 13 + 210);

	glColor3f(1.0, 1.0, 1.0);
	glVertex2f(39, 93 + 210);
	glVertex2f(42, 93 + 210);
	glVertex2f(60, 93 + 210);
	glVertex2f(63, 93 + 210);

	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(45, 79 + 210);
	glVertex2f(48, 79 + 210);

	glVertex2f(45, 78 + 210);
	glVertex2f(48, 78 + 210);

	glVertex2f(48, 77 + 210);
	glVertex2f(53, 77 + 210);

	glVertex2f(48, 76 + 210);
	glVertex2f(53, 76 + 210);

	glVertex2f(53, 79 + 210);
	glVertex2f(58, 79 + 210);

	glVertex2f(53, 78 + 210);
	glVertex2f(58, 78 + 210);


	glVertex2f(53, 83 + 210);
	glVertex2f(58, 83 + 210);

	glVertex2f(53, 84 + 210);
	glVertex2f(58, 84 + 210);

	glVertex2f(58, 77 + 210);
	glVertex2f(63, 77 + 210);
	glVertex2f(58, 76 + 210);
	glVertex2f(63, 76 + 210);
	glVertex2f(63, 78 + 210);
	glVertex2f(66, 78 + 210);
	glVertex2f(63, 79 + 210);
	glVertex2f(66, 79 + 210);

	//hand1
	glVertex2f(31, 49 + 210);
	glVertex2f(31, 46 + 210);
	glVertex2f(32, 49 + 210);
	glVertex2f(32, 46 + 210);
	glVertex2f(33, 46 + 210);
	glVertex2f(33, 40 + 210);
	glVertex2f(34, 46 + 210);
	glVertex2f(34, 40 + 210);
	glVertex2f(33, 39 + 210);
	glVertex2f(39, 39 + 210);
	glVertex2f(33, 38 + 210);
	glVertex2f(39, 38 + 210);
	glVertex2f(39, 37 + 210);
	glVertex2f(45, 37 + 210);
	glVertex2f(39, 36 + 210);
	glVertex2f(45, 36 + 210);

	glVertex2f(45, 38 + 210);
	glVertex2f(45, 44 + 210);
	glVertex2f(46, 38 + 210);
	glVertex2f(46, 44 + 210);
	glVertex2f(44, 42 + 210);
	glVertex2f(44, 48 + 210);
	glVertex2f(43, 42 + 210);
	glVertex2f(43, 48 + 210);
	glVertex2f(42, 48 + 210);
	glVertex2f(42, 51 + 210);
	glVertex2f(41, 48 + 210);
	glVertex2f(41, 51 + 210);

	//2nd

	glVertex2f(31 + 39, 49 + 210);
	glVertex2f(31 + 39, 46 + 210);
	glVertex2f(32 + 39, 49 + 210);
	glVertex2f(32 + 39, 46 + 210);
	glVertex2f(33 + 36, 46 + 210);
	glVertex2f(33 + 36, 40 + 210);
	glVertex2f(34 + 36, 46 + 210);
	glVertex2f(34 + 36, 40 + 210);
	glVertex2f(33 + 30, 39 + 210);
	glVertex2f(39 + 30, 39 + 210);
	glVertex2f(33 + 30, 38 + 210);
	glVertex2f(39 + 30, 38 + 210);
	glVertex2f(39 + 18, 37 + 210);
	glVertex2f(45 + 18, 37 + 210);
	glVertex2f(39 + 18, 36 + 210);
	glVertex2f(45 + 18, 36 + 210);

	glVertex2f(45 + 10, 38 + 210);
	glVertex2f(45 + 10, 44 + 210);
	glVertex2f(46 + 10, 38 + 210);
	glVertex2f(46 + 10, 44 + 210);
	glVertex2f(44 + 16, 42 + 210);
	glVertex2f(44 + 16, 48 + 210);
	glVertex2f(43 + 16, 42 + 210);
	glVertex2f(43 + 16, 48 + 210);
	glVertex2f(42 + 22, 48 + 210);
	glVertex2f(42 + 22, 51 + 210);
	glVertex2f(41 + 22, 48 + 210);
	glVertex2f(41 + 22, 51 + 210);

	//tail
	glVertex2f(82, 78 + 210);
	glVertex2f(85, 78 + 210);
	glVertex2f(82, 79 + 210);
	glVertex2f(85, 79 + 210);

	glVertex2f(85, 80 + 210);
	glVertex2f(88, 80 + 210);
	glVertex2f(85, 81 + 210);
	glVertex2f(88, 81 + 210);

	glVertex2f(91, 82 + 210);
	glVertex2f(88, 82 + 210);
	glVertex2f(91, 83 + 210);
	glVertex2f(88, 83 + 210);

	glVertex2f(91, 84 + 210);
	glVertex2f(100, 84 + 210);
	glVertex2f(91, 85 + 210);
	glVertex2f(100, 85 + 210);

	glVertex2f(100, 85 + 210);
	glVertex2f(100, 67 + 210);
	glVertex2f(101, 85 + 210);
	glVertex2f(101, 67 + 210);

	glVertex2f(100, 67 + 210);
	glVertex2f(97, 67 + 210);
	glVertex2f(100, 66 + 210);
	glVertex2f(97, 66 + 210);

	glVertex2f(97, 66 + 210);
	glVertex2f(97, 60 + 210);
	glVertex2f(96, 66 + 210);
	glVertex2f(96, 60 + 210);
	y = 60; x = 93;
	for (int i = 0; i < 3; ++i)
	{
		glVertex2f(x, y + 210);
		glVertex2f(x + 3, y + 210);
		glVertex2f(x, y - 1 + 210);
		glVertex2f(x + 3, y - 1 + 210);
		y -= 2;
		x += 3;
	}

	y -= 1;
	for (int i = 0; i < 7; ++i) {
		glVertex2f(x, y + 210);
		glVertex2f(x - 3, y + 210);
		glVertex2f(x, y - 1 + 210);
		glVertex2f(x - 3, y - 1 + 210);
		x -= 3;
		y -= 2;

	}
	glEnd();
	glLineWidth(3);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	// the numbers after every point is a reference to a paper to trace the points and know what i've drawn and what i haven't
	glVertex2d(0.35 * 64 + 450, 0.35 * 196 + 210);//1
	glVertex2d(0.35 * 83 + 450, 0.35 * 188 + 210);//2
	glVertex2d(0.35 * 69 + 450, 0.35 * 129 + 210);//3
	glVertex2d(0.35 * 103 + 450, 0.35 * 104 + 210);//4
	glVertex2d(0.35 * 153 + 450, 0.35 * 94 + 210);//5
	glVertex2d(0.35 * 182 + 450, 0.35 * 142 + 210);//6
	glVertex2d(0.35 * 130 + 450, 0.35 * 297 + 210);//7
	glVertex2d(0.35 * 89 + 450, 0.35 * 266 + 210);//8
	glVertex2d(0.35 * 68 + 450, 0.35 * 229 + 210);//9
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2d(0.35 * 64 + 450, 0.35 * 196 + 210);//1
	glVertex2d(0.35 * 48 + 450, 0.35 * 188 + 210);//10
	glVertex2d(0.35 * 69 + 450, 0.35 * 129 + 210);//3
	glVertex2d(0.35 * 83 + 450, 0.35 * 188 + 210);//2
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2d(0.35 * 50 + 450, 0.35 * 181 + 210);//11
	glVertex2d(0.35 * 58 + 450, 0.35 * 179 + 210);//12
	glVertex2d(0.35 * 53 + 450, 0.35 * 171 + 210);//13
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2d(0.35 * 83 + 450, 0.35 * 188 + 210);//2
	glVertex2d(0.35 * 97 + 450, 0.35 * 151 + 210);//14
	glVertex2d(0.35 * 128 + 450, 0.35 * 143 + 210);//15
	glVertex2d(0.35 * 143 + 450, 0.35 * 161 + 210);//16
	glVertex2d(0.35 * 144 + 450, 0.35 * 203 + 210);//17
	glVertex2d(0.35 * 104 + 450, 0.35 * 232 + 210);//18
	glEnd();

	glBegin(GL_LINES);
	glVertex2d(0.35 * 182 + 450, 0.35 * 142 + 210);//6
	glVertex2d(0.35 * 144 + 450, 0.35 * 203 + 210);//17
	glEnd();


	glBegin(GL_POLYGON);
	glVertex2d(0.35 * 130 + 450, 0.35 * 297 + 210);//7
	glVertex2d(0.35 * 132 + 450, 0.35 * 324 + 210);//19
	glVertex2d(0.35 * 149 + 450, 0.35 * 332 + 210);//20
	glVertex2d(0.35 * 170 + 450, 0.35 * 322 + 210);//21
	glVertex2d(0.35 * 178 + 450, 0.35 * 313 + 210);//22
	glVertex2d(0.35 * 172 + 450, 0.35 * 301 + 210);//23
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2d(0.35 * 172 + 450, 0.35 * 301 + 210);//23
	glVertex2d(0.35 * 178 + 450, 0.35 * 313 + 210);//22
	glVertex2d(0.35 * 195 + 450, 0.35 * 316 + 210);//24
	glVertex2d(0.35 * 216 + 450, 0.35 * 301 + 210);//25
	glVertex2d(0.35 * 218 + 450, 0.35 * 292 + 210);//26
	glVertex2d(0.35 * 221 + 450, 0.35 * 276 + 210);//27
	glVertex2d(0.35 * 212 + 450, 0.35 * 267 + 210);//28
	glVertex2d(0.35 * 196 + 450, 0.35 * 273 + 210);//29
	glVertex2d(0.35 * 183 + 450, 0.35 * 284 + 210);//30
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2d(0.35 * 182 + 450, 0.35 * 142 + 210);//6
	glVertex2d(0.35 * 130 + 450, 0.35 * 297 + 210);//7
	glVertex2d(0.35 * 172 + 450, 0.35 * 301 + 210);//23
	glVertex2d(0.35 * 183 + 450, 0.35 * 284 + 210);//30
	glVertex2d(0.35 * 196 + 450, 0.35 * 273 + 210);//29
	glVertex2d(0.35 * 212 + 450, 0.35 * 267 + 210);//28
	glVertex2d(0.35 * 221 + 450, 0.35 * 276 + 210);//27
	glVertex2d(0.35 * 241 + 450, 0.35 * 219 + 210);//31
	glVertex2d(0.35 * 225 + 450, 0.35 * 164 + 210);//32
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2d(0.35 * 241 + 450, 0.35 * 219 + 210);//31
	glVertex2d(0.35 * 221 + 450, 0.35 * 276 + 210);//27
	glVertex2d(0.35 * 218 + 450, 0.35 * 292 + 210);//26
	glVertex2d(0.35 * 283 + 450, 0.35 * 340 + 210);//33
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2d(0.35 * 225 + 450, 0.35 * 164 + 210);//32
	glVertex2d(0.35 * 241 + 450, 0.35 * 219 + 210);//31
	glVertex2d(0.35 * 283 + 450, 0.35 * 340 + 210);//33
	glVertex2d(0.35 * 289 + 450, 0.35 * 267 + 210);//34
	glVertex2d(0.35 * 308 + 450, 0.35 * 185 + 210);//35
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2d(0.35 * 153 + 450, 0.35 * 94 + 210);//5
	glVertex2d(0.35 * 182 + 450, 0.35 * 142 + 210);//6
	glVertex2d(0.35 * 225 + 450, 0.35 * 164 + 210);//32
	glVertex2d(0.35 * 308 + 450, 0.35 * 185 + 210);//35
	glVertex2d(0.35 * 294 + 450, 0.35 * 115 + 210);//36
	glVertex2d(0.35 * 269 + 450, 0.35 * 85 + 210);//37
	glVertex2d(0.35 * 199 + 450, 0.35 * 64 + 210);//38
	glVertex2d(0.35 * 150 + 450, 0.35 * 82 + 210);//39
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2d(0.35 * 283 + 450, 0.35 * 340 + 210);//33
	glVertex2d(0.35 * 289 + 450, 0.35 * 267 + 210);//34
	glVertex2d(0.35 * 308 + 450, 0.35 * 185 + 210);//35
	glVertex2d(0.35 * 400 + 450, 0.35 * 239 + 210);//40
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2d(0.35 * 283 + 450, 0.35 * 340 + 210);//33
	glVertex2d(0.35 * 400 + 450, 0.35 * 239 + 210);//40
	glVertex2d(0.35 * 393 + 450, 0.35 * 343 + 210);//41
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2d(0.35 * 400 + 450, 0.35 * 239 + 210);//40
	glVertex2d(0.35 * 393 + 450, 0.35 * 343 + 210);//41
	glVertex2d(0.35 * 422 + 450, 0.35 * 248 + 210);//42
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2d(0.35 * 393 + 450, 0.35 * 343 + 210);//41
	glVertex2d(0.35 * 422 + 450, 0.35 * 248 + 210);//42
	glVertex2d(0.35 * 436 + 450, 0.35 * 280 + 210);//43
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2d(0.35 * 422 + 450, 0.35 * 248 + 210);//42
	glVertex2d(0.35 * 436 + 450, 0.35 * 280 + 210);//43
	glVertex2d(0.35 * 451 + 450, 0.35 * 256 + 210);//44
	glVertex2d(0.35 * 454 + 450, 0.35 * 222 + 210);//45
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2d(0.35 * 436 + 450, 0.35 * 280 + 210);//43
	glVertex2d(0.35 * 451 + 450, 0.35 * 256 + 210);//44
	glVertex2d(0.35 * 460 + 450, 0.35 * 265 + 210);//46
	glVertex2d(0.35 * 460 + 450, 0.35 * 280 + 210);//47
	glVertex2d(0.35 * 446 + 450, 0.35 * 296 + 210);//48
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2d(0.35 * 400 + 450, 0.35 * 239 + 210);//40
	glVertex2d(0.35 * 422 + 450, 0.35 * 248 + 210);//42
	glVertex2d(0.35 * 454 + 450, 0.35 * 222 + 210);//45
	glVertex2d(0.35 * 452 + 450, 0.35 * 155 + 210);//49
	glVertex2d(0.35 * 397 + 450, 0.35 * 137 + 210);//50
	glVertex2d(0.35 * 360 + 450, 0.35 * 187 + 210);//51
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2d(0.35 * 397 + 450, 0.35 * 137 + 210);//50
	glVertex2d(0.35 * 452 + 450, 0.35 * 155 + 210);//49
	glVertex2d(0.35 * 421 + 450, 0.35 * 94 + 210);//52
	glVertex2d(0.35 * 369 + 450, 0.35 * 66 + 210);//53
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2d(0.35 * 360 + 450, 0.35 * 187 + 210);//51
	glVertex2d(0.35 * 397 + 450, 0.35 * 137 + 210);//50
	glVertex2d(0.35 * 369 + 450, 0.35 * 66 + 210);//53
	glVertex2d(0.35 * 377 + 450, 0.35 * 117 + 210);//54
	glVertex2d(0.35 * 359 + 450, 0.35 * 147 + 210);//61
	glVertex2d(0.35 * 327 + 450, 0.35 * 161 + 210);//55
	glVertex2d(0.35 * 314 + 450, 0.35 * 147 + 210);//56
	glVertex2d(0.35 * 316 + 450, 0.35 * 168 + 210);//59
	glVertex2d(0.35 * 332 + 450, 0.35 * 183 + 210);//60
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2d(0.35 * 369 + 450, 0.35 * 66 + 210);//53
	glVertex2d(0.35 * 377 + 450, 0.35 * 117 + 210);//54
	glVertex2d(0.35 * 359 + 450, 0.35 * 147 + 210);//61
	glVertex2d(0.35 * 327 + 450, 0.35 * 161 + 210);//55
	glVertex2d(0.35 * 314 + 450, 0.35 * 147 + 210);//56
	glVertex2d(0.35 * 346 + 450, 0.35 * 70 + 210);//57
	glEnd();

	glBegin(GL_LINES);
	glVertex2d(0.35 * 346 + 450, 0.35 * 70 + 210);//57
	glVertex2d(0.35 * 300 + 450, 0.35 * 72 + 210);//58
	glVertex2d(0.35 * 300 + 450, 0.35 * 72 + 210);//58
	glVertex2d(0.35 * 269 + 450, 0.35 * 85 + 210);//37
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	glVertex2d(0.35 * 113 + 450, 0.35 * 155 + 210);
	glVertex2d(0.35 * 132 + 450, 0.35 * 165 + 210);
	glVertex2d(0.35 * 132 + 450, 0.35 * 185 + 210);
	glVertex2d(0.35 * 115 + 450, 0.35 * 195 + 210);
	glVertex2d(0.35 * 97 + 450, 0.35 * 185 + 210);
	glVertex2d(0.35 * 97 + 450, 0.35 * 165 + 210);
	glVertex2d(0.35 * 113 + 450, 0.35 * 155 + 210);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0, 1, 0.12);
	glVertex2d(0.35 * 116 + 450, 0.35 * 156 + 210);
	glVertex2d(0.35 * 126 + 450, 0.35 * 162 + 210);
	glVertex2d(0.35 * 126 + 450, 0.35 * 175 + 210);
	glVertex2d(0.35 * 116 + 450, 0.35 * 180 + 210);
	glVertex2d(0.35 * 105 + 450, 0.35 * 175 + 210);
	glVertex2d(0.35 * 105 + 450, 0.35 * 162 + 210);
	glVertex2d(0.35 * 116 + 450, 0.35 * 156 + 210);
	glEnd();



	// TODO
}

void Init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

void Reshape(int width, int height)
{
	::width = width;
	::height = height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	DrawGrid();
}

void Reset()
{
	gameOver = tie = turn = false;
	for (int i = 0; i < 9; i++) {
		plays[i / 3][i % 3] = -1;
	}
	for (int i = 0; i < 9; i++) {
		vecCells[i].clicked = true;
	}
	sleep = 0;
	verg = 0;
}

void Display()
{////////////////please make it fullscreen
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);

	if (gameOver)
	{
		string str = playerNames[winner] + " wins. Congratulations!";
		renderBitmapString(0.25 * width, 0.1 * height, 0, GLUT_BITMAP_TIMES_ROMAN_24, str.c_str());


	}
	if (tie)
		renderBitmapString(0.48 * width, 0.1 * height, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Tie");
	if (turn) {
		Point2D point(460, 200);
		Rectangle r(point, 180, 200);
		r.Draw(clrPlayer2);
	}
	else {
		Point2D point(20, 200);
		Rectangle r(point, 100, 200);
		r.Draw(clrPlayer1);
	}
	glColor3f(1.0, 0.0, 0.0);
	renderBitmapString(20, 150, 0, GLUT_BITMAP_TIMES_ROMAN_24, playerNames[player_2].c_str());
	renderBitmapString(480, 150, 0, GLUT_BITMAP_TIMES_ROMAN_24, playerNames[player_1].c_str());
	DrawPlayer(Rectangle(Point2D(0.02 * width, 0.3 * height), 0.2 * width, 0.25 * height), player_1);
	//DrawPlayer(Rectangle(Point2D(width - 0.02 * width - 0.2 * width, 0.3 * height), 0.2 * width, 0.25 * height), player_2);

	if (sleep)sleepy();

	// draw 3x3 grid
	for (unsigned int i = 0; i < vecCells.size(); ++i)
	{
		vecCells[i].Draw(clrRecBackGrd);
	}
	if (verg) makeVertex();
	glFlush();
}


void CheckWin()
{
	int whoWon;
	int A[2] = { 0,1 };
	for (int i = 0; i < 2; i++) {
		if (A[i] == plays[0][0] && A[i] == plays[1][1] && plays[2][2] == A[i]) {
			gameOver = true;
			whoWon = i;
		}
		if (A[i] == plays[0][2] && A[i] == plays[1][1] && plays[2][0] == A[i]) {
			gameOver = true;
			whoWon = i;
		}
		for (int j = 0; j < 3; j++) {
			if (A[i] == plays[j][1] && A[i] == plays[j][0] && A[i] == plays[j][2])
			{
				gameOver = true;
				whoWon = i;
			}
		}
		for (int j = 0; j < 3; j++) {
			if (A[i] == plays[0][j] && A[i] == plays[1][j] && A[i] == plays[2][j])
			{
				gameOver = true;
				whoWon = i;

			}
		}
	}
	if (gameOver) {
		winner = whoWon;
		verg = 1;
		if (!whoWon) sleep = 1;

	}
	// TODO: check for a win
}

bool CheckTie()
{
	int num = 0;
	for (int i = 0; i < 9; i++) {
		if (!vecCells[i].clicked)
			num++;
	}
	if (num == 9 && winner == -1) {
		tie = true;
	}
	// TODO: check for tie

	return false;
}

void MouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !gameOver && !tie) {
		Point2D pnt(x, height - y);
		for (int i = 0; i < vecCells.size(); i++) {
			if (vecCells[i].clicked) {
				if (vecCells[i].IsInside(pnt))
				{
					if (!turn) {
						vecCells[i].whoClicked = true;
						plays[i / 3][i % 3] = 1;
					}
					else {
						vecCells[i].whoClicked = false;
						plays[i / 3][i % 3] = 0;
					}
					vecCells[i].clicked = false;
					if (turn)
						turn = false;
					else
						turn = true;
					CheckWin();
					CheckTie();
				}
			}
		}
		// TODO: Most of your work goes here
	}
}

void ProcessNormalKeys(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	else if (key == 'R' || key == 'r')
		Reset();
}

void main(int argc, char **argv)
{
	cout << "Enter Name of Player 1 : ";
	cin >> playerNames[player_2];
	cout << "Enter Name of Player 2 : ";
	cin >> playerNames[player_1];

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Homework 1: Tic Tac Toe Game");
	for (int i = 0; i < vecCells.size(); i++) {
		vecCells[i].clicked = false;
	}

	Init();


	// register callbacks
	glutDisplayFunc(Display);

	glutReshapeFunc(Reshape);
	glutIdleFunc(Display);

	glutMouseFunc(MouseClick);

	glutKeyboardFunc(ProcessNormalKeys);

	// enter GLUT event processing cycle
	glutMainLoop();
}
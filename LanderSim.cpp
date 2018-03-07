#include <iostream>
#include <iomanip>
#include <math.h>
#include "LunarLander.hpp"
#include <time.h>
#include <fstream>
#include <string>
#include <cstdlib>

#define INTERACTIVE 1 // Set to 1 for text-based coolness, otherwise set it to 0.
#define SROWS 32      // how tall the screen is
#define SCOLS 80      // how wide the screen is

using namespace std;

// did the client type any l's or L's?  Let's count.
int ells(string input) {
  int l = 0;

  for (int i = 0; i < input.length(); ++i) {
    if (input[i] == 'l') {
      l++;
    }

    if (input[i] == 'L') {
      l += 10;
    }
  }

  return l;
}

// did the client type any r's or R's?  Let's count.
int arrs(string input) {
  int r = 0;

  for (int i = 0; i < input.length(); ++i) {
    if (input[i] == 'r') {
      r++;
    }

    if (input[i] == 'R') {
      r += 10;
    }
  }

  return r;
}

// did the client type any b's or B's?  Let's count.
int bees(string input) {
  int b = 0;

  for (int i = 0; i < input.length(); ++i) {
    if (input[i] == 'b') {
      b++;
    }

    if (input[i] == 'B') {
      b += 10;
    }
  }

  return b;
}

// have the user type in a possibly empty command line
// and make the lander do what the user says
void getCommands(Lander &l) {
  string input;

  if (INTERACTIVE) {
    // if INTERACTIVE mode is set, get some commands
    getline(cin, input);
    l.burn(bees(input));
    l.rotateLeft(ells(input) * PI / 180.0);
    l.rotateRight(arrs(input) * PI / 180.0);
  } else {
    // put your autopilot software here if you feel up to the challenge
    // otherwise don't bother
    return;
  }
}

// print out information about the lander's status
void dashboard(Lander l, int frame) {
  int r, c;
  int cc;
  Vect p, v;
  double a;
  double f;
  static bool rock[SCOLS];
  static bool rockset = false;
  int landerLine = 0;
  int rockLine = 0;

  // make some rocks
  if (!rockset) {
    rockset = true;

    for (int j = 0; j < SCOLS; ++j) {
      rock[j] = false;
    }

    for (int j = 0; j < SCOLS / 1.5; ++j) {
      rock[rand() % SCOLS] = true;
    }
  }

  // get the lander status
  p = l.getPosition();
  v = l.getVelocity();
  a = l.getAngle();
  f = l.getFuel();

  // make a character array that looks like what we want on the screen
  static char screen[SROWS][SCOLS];
  for (int i = 0; i < SROWS; i++) {
    for (int j = 0; j < SCOLS; j++) {
      screen[i][j] = ' ';
    }
  }

  for (int j = 0; j < SCOLS; j++) {
    screen[SROWS - 1][j] = '-';
  }

  for (int j = 0; j < SCOLS; j++) {
    if (rock[j]) {
      screen[SROWS - 1][j] = '^';
    }
  }

  // figure out where the lander is and 'draw' it into the array
  r = SROWS - (int) (p.y / 10.0);

  if (r >= SROWS) {
    r = SROWS - 1;
  }

  c = (int) (p.x / 10.0);

  if (r >= 0 && r < SROWS && c >= 0 && c < SCOLS) {
    screen[r][c] = '*';
    landerLine = r;

    // draw the direction indicator
    if (a >= (1.875 * PI) || a < (0.125 * PI)) {
      if (c < SCOLS - 1) {
        screen[r][c + 1] = '-';
      }
    }

    if (a >= (0.125 * PI) && a < (0.375 * PI)) {
      if (c < SCOLS - 1 && r > 0) {
        screen[r - 1][c + 1] = '/';
      }
    }

    if (a >= (0.375 * PI) && a < (0.625 * PI)) {
      if (r > 0) {
        screen[r - 1][c] = '|';
      }
    }

    if (a >= (0.625 * PI) && a < (0.875 * PI)) {
      if (c > 0 && r > 0) {
        screen[r - 1][c - 1] = '\\';
      }
    }

    if (a >= (0.875 * PI) && a < (1.125 * PI)) {
      if (c > 0) {
        screen[r][c - 1] = '-';
      }
    }

    if (a >= (1.125 * PI) && a < (1.375 * PI)) {
      if (c > 0 && r < SROWS - 1) {
        screen[r + 1][c - 1] = '/';
      }
    }

    if (a >= (1.375 * PI) && a < (1.625 * PI)) {
      if (r < SROWS - 1) {
        screen[r + 1][c] = '|';
      }
    }

    if (a >= (1.625 * PI) && a < (1.875 * PI)) {
      if (c < SCOLS - 0 && r < SROWS - 1) {
        screen[r + 1][c + 1] = '\\';
      }
    }
  }
  // use arrows if the lander has flown offsreen
  if (r < 0 && c < 0) {
    screen[0][0] = '^';
    screen[1][0] = '<';
  } else if (r < 0 && c >= SCOLS) {
    screen[0][SCOLS - 1] = '^';
    screen[1][SCOLS - 1] = '>';
  } else if (r < 0) {
    screen[0][c] = '^';
  } else if (c < 0) {
    screen[r][0] = '<';
  } else if (c >= SCOLS) {
    screen[r][SCOLS - 1] = '>';
  }

  cc = c;

  // handle crashes and landings in an interesting way
  if (l.getCrashed()) {
    if (c < 0) {
      c = -1;
    }

    if (r >= 0 && r < SROWS && c >= 0 && c < SCOLS) {
      screen[r][c] = 38;
    }

    r--;
    c++;

    if (c < SCOLS) {
      screen[r][c] = 47;
    }

    r--;
    c++;

    if (c < SCOLS) {
      screen[r][c] = 115;
    }

    c++;

    if (c < SCOLS) {
      screen[r][c] = 112;
    }

    c++;

    if (c < SCOLS) {
      screen[r][c] = 108;
    }

    c++;

    if (c < SCOLS) {
      screen[r][c] = 117;
    }

    c++;

    if (c < SCOLS) {
      screen[r][c] = 100;
    }

    c++;

    if (c < SCOLS) {
      screen[r][c] = 46;
    }
  }

  if (l.getLanded() && rock[c]) {
    if (c < 0) {
      c = -1;
    }

    r--;
    c++;

    if (c < SCOLS) {
      screen[r][c] = 47;
    }

    r--;
    c++;

    if (c < SCOLS) {
      screen[r][c] = 110;
    }

    c++;

    if (c < SCOLS) {
      screen[r][c] = 111;
    }

    c++;

    if (c < SCOLS) {
      screen[r][c] = 111;
    }

    c++;

    if (c < SCOLS) {
      screen[r][c] = 111;
    }

    c++;

    if (c < SCOLS) {
      screen[r][c] = 33;
    }
  } else if (l.getLanded()) {
    if (c < 0) {
      c = -1;
    }

    r--;
    c++;

    if (c < SCOLS) {
      screen[r][c] = 47;
    }

    r--;
    c++;

    if (c < SCOLS) {
      screen[r][c] = 119;
    }

    c++;

    if (c < SCOLS) {
      screen[r][c] = 104;
    }

    c++;

    if (c < SCOLS) {
      screen[r][c] = 101;
    }

    c++;

    if (c < SCOLS) {
      screen[r][c] = 119;
    }

    c++;

    if (c < SCOLS) {
      screen[r][c] = 46;
    }
  }

  if (INTERACTIVE) {
    //if INTERACTIVE mode is set, print out the screen
    //otherwise don't bother
    for (int i = 0; i < SROWS; ++i) {
      bool containsChars = false;
      for (int j = 0; j < SCOLS; ++j) {
        if (screen[i][j] != ' ') {
          containsChars = true;
        }
      }

      if (containsChars) {
        for (int j = 0; j < SCOLS; ++j) {
          cout << screen[i][j];
        }
      }
      cout << endl;
    }
  }

  // always print out the lander status
  cout << "x:" << setw(7) << left << fixed << setprecision(1) << p.x;
  cout << "y:" << setw(7) << left << fixed << setprecision(1) << p.y;
  cout << "vx:" << setw(7) << left << fixed << setprecision(1) << v.x;
  cout << "vy:" << setw(7) << left << fixed << setprecision(1) << v.y;
  cout << "angle:" << setw(7) << left << fixed << setprecision(1) << a * 180.0 / PI;
  cout << "fuel:" << setw(5) << left << fixed << setprecision(0) << f << "  ";
  cout << "status: ";

  if (l.getCrashed()) {
    cout << "CRASHED";
  } else if (l.getLanded() && rock[cc]) {
    cout << "HIT A ROCK";
  } else if (l.getLanded()) {
    cout << "landed";
  } else if (l.getFuel() <= 0.0) {
    cout << "no fuel";
  } else if (l.getFuel() <= 20.0) {
    cout << "low fuel";
  } else {
    cout << "in flight";
  }

  cout << endl;

  cout << "=~=" << endl;
}

void printInstructions() {
  // print out the instruction for INTERACTIVE mode
  char q[3];

  cout << "\n\n\n\n\n\n\n\n\n\n";
  cout << "                           Lunar Lander Simulator\n\n\n";
  cout << "      Enter 'b' to burn one unit of fuel; 'B' to burn 10 units.\n";
  cout << "      Enter 'r' to rotate right 1 degree; 'R' to rotate 10 degrees.\n";
  cout << "      Enter 'l' to rotate left  1 degree; 'L' to rotate 10 degrees.\n";
  cout << "      String together letters to make more complicated commands.\n";
  cout << "      For example, 'RrrbBbr' will rotate right 13 degrees and\n";
  cout << "                           burn 12 units of fuel.\n";
  cout << "\n";
  cout << "                           (hit <enter> to begin)\n\n";
  cout << "=~=" << endl;
  cin.getline(q, 3);
}


int main() {
#ifdef JARVIS
  srand(0);
#else
  srand(time(NULL));
#endif

  int frame = 0;
  double a = PI;
  double f = 200.0;
  double MTV = 5.0;
  bool crashed = false;
  bool landed = false;

  //set up the parameters for a lander
  Vect G, v, p;
  G.x = 0.0;
  G.y = -1.622;
  v.x = 20;
  v.y = 0.0;
  p.x = 100.0;
  p.y = 300.0;

  //create an instance of the lander using the above parameters
  Lander l(G, a, v, f, p, MTV, crashed, landed);

  //throw out some instructions if we are in INTERACTIVE mode
  if (INTERACTIVE) {
    printInstructions();
  }

  //loop once for each second of flight
  while (!l.getCrashed() && !l.getLanded()) {
    dashboard(l, frame);
    getCommands(l);
    l.timeUpdate();
    frame++;
  }

  dashboard(l, frame);

  //all done
  return 0;
}


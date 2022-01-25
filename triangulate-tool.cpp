#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <random>
#include <string>

using namespace cv;
using namespace std;

Mat input_image;
Mat output_image;

int points = 100;
int per_side = 3;

Subdiv2D subdiv;

void run_filter(int, void *) {
  output_image = input_image.clone();

  vector<Vec6f> triangle_list;
  subdiv.getTriangleList(triangle_list);
  for (int i = 0; i < triangle_list.size(); i++) {
    Vec6f &t = triangle_list[i];
    Point pt[3];
    pt[0] = Point(cvRound(t[0]), cvRound(t[1]));
    pt[1] = Point(cvRound(t[2]), cvRound(t[3]));
    pt[2] = Point(cvRound(t[4]), cvRound(t[5]));

    const Point *pts[1] = {pt};
    int npts[1] = {3};
    Mat mask(input_image.size(), CV_8UC1);
    mask = Scalar::all(0);
    fillPoly(mask, pts, npts, 1, Scalar(255));

    Scalar colour = mean(input_image, mask);
    fillPoly(output_image, pts, npts, 1, colour, LINE_AA);
  }
}

int random(int range_from, int range_to) {
  random_device rand_dev;
  mt19937 generator(rand_dev());
  uniform_int_distribution<int> distr(range_from, range_to);
  return distr(generator);
}

int rand_w() { return random(0, input_image.rows - 1); }
int rand_h() { return random(0, input_image.cols - 1); }
Point rand_p() { return Point(rand_h(), rand_w()); }
Point rand_pw(int w_constraint) { return Point(rand_h(), w_constraint); }
Point rand_ph(int h_constraint) { return Point(h_constraint, rand_w()); }

void set_points() {
  // all 4 corners
  subdiv.insert(Point(0, 0));
  subdiv.insert(Point(0, input_image.rows - 1));
  subdiv.insert(Point(input_image.cols - 1, input_image.rows - 1));
  subdiv.insert(Point(input_image.cols - 1, 0));

  // per side points
  for (int n = 0; n < per_side; n++)
    subdiv.insert(rand_pw(0));
  for (int n = 0; n < per_side; n++)
    subdiv.insert(rand_pw(input_image.rows - 1));
  for (int n = 0; n < per_side; n++)
    subdiv.insert(rand_ph(0));
  for (int n = 0; n < per_side; n++)
    subdiv.insert(rand_ph(input_image.cols - 1));

  // random points all over
  for (int n = 0; n < points; n++)
    subdiv.insert(rand_p());
}

void show_help(const char *progname) {
  cerr << "Usage: " << progname
       << " input_image output_image [total_points] [total_points_per_side]"
       << endl;
}

int str_to_int(string arg) {
  try {
    size_t pos;
    int x = stoi(arg, &pos);
    if (pos < arg.size()) {
      cerr << "Trailing chartacters after number: " << arg << endl;
    }
    return x;
  } catch (invalid_argument const &ex) {
    cerr << "Invalid number: " << arg << endl;
  } catch (out_of_range const &ex) {
    cerr << "Number out of range: " << arg << endl;
  }
  exit(1);
}

int main(int argc, char **argv) {
  if (argc < 3) {
    show_help(argv[0]);
    return -1;
  }

  input_image = imread(argv[1], cv::IMREAD_COLOR);
  if (!input_image.data) {
    cerr << "ERROR: Could not open or find the image " << argv[1] << endl;
    return -1;
  }

  if (argc > 3)
    points = str_to_int(argv[3]);
  if (argc > 4)
    per_side = str_to_int(argv[4]);

  subdiv.initDelaunay(Rect(0, 0, input_image.cols, input_image.rows));
  set_points();
  run_filter(0, 0);
  imwrite(argv[2], output_image);
  return 0;
}

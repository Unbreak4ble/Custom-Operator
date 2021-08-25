#include <vector>
#include <string>
#include <ctype.h>
#include <algorithm>

namespace complements {
  double add(double right, double left) {
    return right + left;
  }

  double sub(double right, double left) {
    return right - left;
  }

  double multi(double right, double left) {
    return right * left;
  }

  double power(double right, double left) {
    double res = right;

    for (int i = 1; i < left; i++)
      res *= right;

    return res;
  }

  double div(double right, double left) {
    return right / left;
  }
};

namespace Custor {

  /*******************\
            Structures
	    \*******************/

  struct current {
    double r;
    double l;
    char op;
    double total;
  };

  struct operators {
    char op;
    int index;
    double( * calc)(double right, double left);
  };

  std::vector < operators > vops;

  /*******************\
            Functions
	    \*******************/

  bool opExists(char cop) {
    for (operators op: vops) {
      if (op.op == cop)
        return true;
    }
    return false;
  }

  operators getOp(char cop) {
    operators ops;
    for (operators op: vops) {
      if (op.op == cop)
        ops = op;
    }
    return ops;
  }

  bool validateOps(std::vector < operators > ops) {
    for (operators op: ops) {
      if (!op.calc)
        return false;
      else if (isalpha(op.op) || isdigit(op.op) || op.op == ' ' || op.op == '.' || op.op == '(' || op.op == ')')
        return false;
			else if(op.index < 0)
				return false;
    }

    return true;
  }

  void checkOps() {
    if (!opExists('+'))
      vops.push_back({
        '+',
        0,
        complements::add
      });

    if (!opExists('-'))
      vops.push_back({
        '-',
        0,
        complements::sub
      });

    if (!opExists('*'))
      vops.push_back({
        '*',
        1,
        complements::multi
      });

    if (!opExists('/'))
      vops.push_back({
        '/',
        1,
        complements::div
      });

    if (!opExists('^'))
      vops.push_back({
        '^',
        2,
        complements::power
      });

  }

  double calc(current cu) {
    double res = 0;

    if (opExists(cu.op))
      res = getOp(cu.op).calc(cu.r, cu.l);

    return res;
  };

  bool sameOp(std::vector < current > data) {
    int currentIdx = -1;
    for (int i = 0; i < data.size(); i++) {
      int idx = getOp(data[i].op).index;
      if (currentIdx < 0)
        currentIdx = idx;
      else if (currentIdx != idx)
        return false;
    }

    return true;
  }

  double reduce(std::vector < current > data) {
    double res = 0.0;

    if (sameOp(data)) {
      for (int i = 0; i < data.size(); i++) {
        res = calc(data[i]);
        if (i < data.size()) {
          data[i + 1].r = res;
        }
      }
    } else {

      for (int i = 0; i < data.size(); i++) {
        current vec = data[i];

        int idx = getOp(vec.op).index;

        if (i < data.size() && getOp(data[i + 1].op).index <= idx) {
          data[i + 1].r = calc(vec);
          data[i + 1].total = calc(data[i + 1]);
        }

        if (i > 0 && getOp(data[i - 1].op).index <= idx) {
          data[i - 1].l = calc(vec);
          data[i - 1].total = calc(data[i - 1]);

        }

        if ((i == data.size() - 1 || (i < data.size() && getOp(data[i + 1].op).index <= idx)) && (i == 0 || (i > 0 && getOp(data[i - 1].op).index <= idx))) {
          data.erase(data.begin() + i);
          i = 0;
        }

        if (sameOp(data))
        res = reduce(data);

        if (data.size() == 1)
          res = reduce(data);

      }
    }

    return res;
  }

  bool isNaN(std::string str) {
    bool is;
    int exprs = 0;
    bool alreadyNum;

    for (int i = 0; i < str.length(); i++) {

      switch (str[i]) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '.':
        if (exprs > 0 && exprs < 2)
          is = true;

        alreadyNum = true;
        break;

      default:
        if (opExists(str[i])) {
          if (alreadyNum == true)
            ++exprs;
					else if(str[i] != '-')
						return true;

        }
        break;
      }

    }

    return is;
  }

  double input(std::string str) {
    double res = 0.0;
    std::vector < current > data;
    current cu {
      0,
      0,
      ' '
    };
    int level = 0, slevel = 0, block = 0;
    std::string joined;
    std::string sblock;

    for (int i = 0; i <= str.length(); i++) {

      if (i == str.length()) {
      if((level > 0 && joined.length() == 0) || block > 0 || (cu.op == ' ' && sblock.length() == 0 && joined.length() == 0))
          throw std::string("Invalid expression");
        else if (sblock.length() > 0)
          joined = input(sblock);
        else if (joined.length() > 0 && cu.op == ' ')
          res = atof(joined.c_str());
        else {
         
          cu.l = (double) atof(joined.c_str());
          cu.total = calc(cu);

          data.push_back(cu);
          joined = "";
        }
      } else {
        if (block == 0) {
          switch (str[i]) {
          case '0':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9':
          case '.':

            if (slevel >= 1 && joined.length() > 0)
              throw std::string("Invalid expression");
            else if (level == 1)
              ++level;

            joined += str[i];
            break;

          case ' ':
            ++slevel;
            break;

          case '(':
            ++block;
            if (level == 1) ++level;
            break;

          case ')':
            throw std::string("Unexpected token '") + str[i] + "'";
            break;

          default:
            if (opExists(str[i])) {
            
              if (joined.length() == 0)
                throw std::string("Invalid expression");

              if (level == 0) {
                cu.r = (double) atof(joined.c_str());
                joined = "";
                slevel = 0;

								
								} else if (level == 2) {
                cu.l = (double) atof(joined.c_str());
                cu.total = calc(cu);
                data.push_back(cu);

                cu = {
                  cu.l,
                  0,
                  ' '
                };
                joined = "";
                level = 0;
                slevel = 0;
                block = 0;
                sblock = "";
              }
							
              if(level < 1){
								cu.op = str[i];
               ++level;
									}
							else
                throw std::string("Invalid expression");

            } else
              throw std::string("Invalid token '") + str[i] + "'";
            break;
          }

        } else {

          if (str[i] == '(') {
            ++block;
            sblock += str[i];
          } else if (str[i] == ')') {
            --block;

            if (block > 0)
              sblock += str[i];
            else {
              
              if (isNaN(sblock) == true)
                joined = std::to_string(input(sblock));
              else
                joined = sblock;

              cu.l = atof(joined.c_str());
              cu.total = calc(cu);
              sblock = "";

            }
          } else
            sblock += str[i];
						

        }

      }

    }

    if (data.size() == 1)
      res = (double) data[0].total;
    else if (data.size() > 0)
      res = reduce(data);

    return res;
  };

  double input(std::string str, std::vector < operators > ops) {
    if (ops.size() > 0) {
      if (validateOps(ops)) {
        vops = ops;
        checkOps();
      } else
        throw std::string("invalid operator struct");
    }

    return input(str);
  };
}

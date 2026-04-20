#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

namespace ToolTest{

       std::string GetCurrentTime()
       {
              auto now = std::chrono::system_clock::now();
              auto in_time_t = std::chrono::system_clock::to_time_t(now);
              std::stringstream ss;
              ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
              return ss.str();
       }

       std::vector<int> SplitVersionByDot(const std::string& version, char split)
       {
              std::vector<int> versionsCode;
              std::stringstream ss(version);
              std::string vStr = "";
              while(std::getline(ss, vStr, split))
              {
                     if (vStr.empty())
                            versionsCode.push_back(0);
                     else
                            versionsCode.push_back(std::stoi(vStr));
              } 
              return versionsCode;
       }

       int CompareVersion(const std::string& vCurrent, const std::string& vNew, char split)
       {
              std::vector<int> v1 = SplitVersionByDot(vCurrent, split);
              std::vector<int> v2 = SplitVersionByDot(vNew, split);

              // 取两者的最大长度
              size_t maxSize = std::max(v1.size(), v2.size());

              for (size_t i = 0; i < maxSize; ++i) 
              {
                     // 优化点：如果索引超出范围，则取 0，无需手动补齐 vector
                     int val1 = (i < v1.size()) ? v1[i] : 0;
                     int val2 = (i < v2.size()) ? v2[i] : 0;

                     if (val1 < val2) return 1;  // vNew 更大
                     if (val1 > val2) return -1; // vCurrent 更大
              }

              return 0; // 完全相等
       }
}
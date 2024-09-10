#include "include/cppjieba/Jieba.hpp"
#include <fstream>
#include "windows.h"
using namespace std;

const char* const DICT_PATH = "../../dict/jieba.dict.utf8";
const char* const HMM_PATH = "../../dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../../dict/user.dict.utf8";
const char* const IDF_PATH = "../../dict/idf.utf8";
const char* const STOP_WORD_PATH = "../../dict/stop_words.utf8";

string Original_text;
string Ask_text;
string output_path;
map<string, int> freq1, freq2;


void readfile(string& text, string path) {
    ifstream fstream;
    fstream.open(path);
    if (fstream.is_open()) {
        string line;
        while (getline(fstream, line)) {
            //cout<<line<<endl;
            text += line;
        }
    }
    else {
        cout << "Failed to open the file" << endl;
        return;
    }
    //cout<<text<<endl;
    cout << "read over" << endl;
    fstream.close();
}
void OutputFile(string path, double ans) {
    ofstream outputFile;
    outputFile.open(path);
    if (outputFile.is_open()) {
        outputFile << ans;
    }
    else {
        cout << "Failed to open the file" << endl;
        return;
    }
    cout << "output over" << endl;
    outputFile.close();
}

/*
        计算词频：
        找个map统计出现次数就行
    */
void getWordFrequency(string text, map<string, int>& mp) {
    cppjieba::Jieba jieba(DICT_PATH,
        HMM_PATH,
        USER_DICT_PATH,
        IDF_PATH,
        STOP_WORD_PATH);
    vector<string> words;
    jieba.Cut(text, words, true);
    for (auto it : words) {
        mp[it]++;
    }

}

//求向量模
/*
 对每个单词的频率的平方和开方
*/
double calmo(map<string, int>& mp) {
    double mo = 0.0;
    for (auto it : mp) {
        mo += pow((double)it.second, 2);
    }
    return sqrt(mo);
}

/*
        余弦相似度求解：
            遍历s1的词频，如果在s2中找到该词频dotP就加入二者在该单词上出现频率的乘积
            分别计算二者词频对应的向量模长，dotP除以二者向量模长的乘积
*/

double calSimilarity(map<string, int>& mp1, map<string, int>& mp2) {
    double dotProduct = 0.0;
    for (auto it : mp1) {
        auto it2 = mp2.find(it.first);
        if (it2 != mp2.end()) {
            dotProduct += (double)((double)it.second) * ((double)it2->second);
        }
    }
    double mo1 = calmo(mp1);
    double mo2 = calmo(mp2);
    //cout << dotProduct << endl;
    //cout << mo1 << " " << mo2 << endl;
    
    return dotProduct / (mo1 * mo2);
}



int main(int argc,char** argv)
{
    
    try {
        if (argc < 4) {
            throw string("the input is error");
        }
    }
    catch (string e) {
        cout << e << endl;
        return 0;
    }
    
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);



    {
        //get the text and the path
        string path[3];
        {

            
             vector<string> temp;
            for (int i = 1; i < argc; i++) {
                string temp = "";
                for (int j = 0; argv[i][j]; j++) {
                    temp += argv[i][j];
                }
                path[i - 1] = temp;
                cout << temp << endl;
            }
            
                        //path[0]="D:\\desktop\\test\\rg_homework\\ConsoleApplication1\\orig.txt";
                       // path[1]="D:\\desktop\\test\\rg_homework\\ConsoleApplication1\\orig_0.8_dis_1.txt";
                        //path[2]="D:\\desktop\\test\\rg_homework\\ConsoleApplication1\\output.txt";
        }
        readfile(Original_text, path[0]);
        //cout << Original_text << endl;
        readfile(Ask_text, path[1]);
        output_path = path[2];
    }
    {
        //分词
        getWordFrequency(Original_text, freq1);
        getWordFrequency(Ask_text, freq2);

        //计算相似度
        double ans = calSimilarity(freq1, freq2);
        cout << "ans: " << ans << endl;
        OutputFile(output_path, ans);
    }

    return 0;
}


#include <iostream>

using namespace std;

#include <curl/curl.h>

string buffer = "";

int writer(char *data, size_t size, size_t nmemb)
{
  int result = 0;
  buffer.append(data, size * nmemb);
  result = size * nmemb;
  return result;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
  size_t written;
  written = fwrite(ptr, size, nmemb, stream);
  return written;
}

int main()
{
  //get user's search words
  string searchWords = "";
  string targetURL = "";
  
  cout << "Enter your searchWords: ";
  getline(cin, searchWords);
  
  while(searchWords.find(" ") < searchWords.length())
  {
    int i = searchWords.find(" ");
    string sub = searchWords.substr(i+1,searchWords.length()-i-1);
    searchWords.replace(i, searchWords.length(), "+");
    searchWords = searchWords + sub;
  }
  
  targetURL = "https://www.bing.com/images/search?q=" + searchWords + "&qs=n&form=QBILPG&sp=-1";
  // https://www.bing.com/images/search?q=pineapple+on+beach
  CURL* curl;
  CURLcode res;


  //https://www.google.com/search?q=music+player&tbm=isch
  
  
  
  curl = curl_easy_init();
  if(curl)
  {
    curl_easy_setopt(curl, CURLOPT_URL, targetURL.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }

  
  
  //buffer
  // 
  // cout << "Done" << endl;
  // system("pause");
  // cout << buffer << endl;
  
  int a;
  int b;
  
  a = buffer.find("\"thumb\"");
  a = buffer.find("href", a);
  a = buffer.find("\"", a)+1;
  b = buffer.find("\"", a);
  
  targetURL = buffer.substr(a, b-a);
  
  //figure out if this is jpg, png, ...
  string extention = "";
  a = targetURL.rfind(".");
  extention = targetURL.substr(a, targetURL.length()-a);
  
  //download
  string outputFileName = searchWords+extention;
  curl = curl_easy_init();
  FILE *fp;

  fp = fopen(outputFileName.c_str(), "wb");
  
  curl_easy_setopt(curl, CURLOPT_URL, targetURL.c_str());
  
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA,     fp);
  
  /* Perform the request, res will get the return code */ 
  res = curl_easy_perform(curl);
  
  /* always cleanup */ 
  curl_easy_cleanup(curl);
  
  fclose(fp);
  
  
  
  
  
}





#ifndef REQUEST_H
#define REQUEST_H

#include <HTTPClient.h>

class Request {
 public:
  // HTTP响应结构
  struct Response {
    int statusCode = 0;
    String body;
    String error;
    bool success = false;
    unsigned long responseTime = 0;  // 响应时间(ms)
  };

  // 获取单例实例
  static Request& getInstance();

  // HTTP GET请求
  Response get(const String& url);

  // 设置请求超时时间
  void setTimeout(int timeoutMs = 10000);

  // 设置User-Agent
  void setUserAgent(const String& userAgent);

  // 设置重试次数
  void setRetryCount(int retries = 3);

  // URL编码函数
  static String urlEncode(const String& str);

 private:
  Request() = default;
  ~Request() = default;

  Request(const Request&) = delete;
  Request& operator=(const Request&) = delete;

  int _timeout = 10000;
  int _connectTimeout = 10000;
  int _retryDelay = 10000;
  String _userAgent = "ESP32-Weather-Device/1.0";
  int _retryCount = 3;

  // 内部方法
  Response performRequest(const String& url, int attempt = 0);
};

#endif
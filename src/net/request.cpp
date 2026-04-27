#include "request.h"

#include "net/network_status.h"

namespace {

String redactQueryParam(const String& url, const String& key) {
  String redacted = url;
  int valueStart = redacted.indexOf(key + "=");
  if (valueStart < 0) {
    return redacted;
  }

  valueStart += key.length() + 1;
  int valueEnd = redacted.indexOf('&', valueStart);
  if (valueEnd < 0) {
    valueEnd = redacted.length();
  }

  redacted.remove(valueStart, valueEnd - valueStart);
  redacted = redacted.substring(0, valueStart) + "<redacted>" +
             redacted.substring(valueStart);
  return redacted;
}

}  // namespace

Request& Request::getInstance() {
  static Request instance;
  return instance;
}

String Request::urlEncode(const String& str) {
  String encoded = "";
  char c;
  char code0;
  char code1;

  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == ' ') {
      encoded += '+';
    } else if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
      encoded += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) {
        code0 = c - 10 + 'A';
      }
      encoded += '%';
      encoded += code0;
      encoded += code1;
    }
  }
  return encoded;
}

Request::Response Request::get(const String& url) {
  if (!NetworkStatus().isConnected()) {
    Response response;
    response.error = "Network not available";
    response.success = false;
    return response;
  }

  return performRequest(url, 0);
}

Request::Response Request::performRequest(const String& url, int attempt) {
  HTTPClient _http;
  Response response;
  unsigned long startTime = millis();

  String logUrl = redactQueryParam(url, "key");
  Serial.printf("[HttpClient] GET %s (attempt %d/%d)\n", logUrl.c_str(),
                attempt + 1, _retryCount + 1);

  _http.begin(url);
  _http.setTimeout(_timeout);
  _http.setConnectTimeout(_connectTimeout);
  _http.addHeader("User-Agent", _userAgent);
  _http.addHeader("Accept", "application/json");
  _http.addHeader("Connection", "close");

  int httpCode = _http.GET();
  unsigned long responseTime = millis() - startTime;

  if (httpCode > 0) {
    response.statusCode = httpCode;
    response.responseTime = responseTime;

    if (httpCode == HTTP_CODE_OK) {
      response.body = _http.getString();
      response.success = true;
      Serial.printf("[HttpClient] Success: %d bytes in %lums\n",
                    response.body.length(), responseTime);
    } else {
      response.error = "HTTP Error: " + String(httpCode);
      response.success = false;
      Serial.printf("[HttpClient] HTTP Error: %d in %lums\n", httpCode,
                    responseTime);
    }
  } else {
    response.error = "Connection failed: " + _http.errorToString(httpCode);
    response.success = false;
    Serial.printf("[HttpClient] Connection failed: %s in %lums\n",
                  _http.errorToString(httpCode).c_str(), responseTime);
    // 重试逻辑
    if (attempt < _retryCount) {
      _http.end();
      delay(_retryDelay * (attempt + 1));  // 递增延迟
      return performRequest(url, attempt + 1);
    }
  }

  _http.end();

  return response;
}

void Request::setTimeout(int timeoutMs) {
  _timeout = timeoutMs;
  Serial.printf("[HttpClient] Timeout set to %dms\n", _timeout);
}

void Request::setUserAgent(const String& userAgent) {
  _userAgent = userAgent;
  Serial.printf("[HttpClient] User-Agent set to: %s\n", _userAgent.c_str());
}

void Request::setRetryCount(int retries) {
  _retryCount = retries;
  Serial.printf("[HttpClient] Retry count set to: %d\n", _retryCount);
}

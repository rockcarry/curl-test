#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

typedef struct {
    char *buf;
    int   len;
    int   pos;
} MEMBUF;

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    MEMBUF *pbuf  = userdata;
    size_t  total = size * nmemb;
    size_t  copy;
    if (pbuf->pos + total < pbuf->len) copy = total;
    else copy = pbuf->len - pbuf->pos;
    memcpy(pbuf->buf + pbuf->pos, ptr, copy);
    pbuf->pos += copy;
    return total;
}

int http_post_send(char *token, char *uid, char *sid, char *point, char *devname,
                           char *ports, int rtspport, int audioport, int auth,
                           char *retstr, int retsize)
{
    CURL    *curl   = NULL;
    MEMBUF   membuf = { retstr, retsize };
    char     poststr[256];
    int      ret;
    snprintf(poststr, sizeof(poststr)-1,
            "token=%s&uid=%s&sid=%s&point=%s&name=%s&ports=%s&rtspPort=%d&audioPort=%d&isAuth=%d",
            token, uid, sid, point, devname, ports, rtspport, audioport, auth);
    printf("%s\n", poststr);
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, "https://www.dbpush.com/cloudMeet/device/pairing");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, poststr);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA    , &membuf);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, FALSE);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    ret = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return ret;
}

#if 1
int main(int argc, char *argv[])
{
    char response[256];
    int  ret;

    curl_global_init(CURL_GLOBAL_WIN32);

    ret = http_post_send("cc194a05ec7d48e08fcebf920af7bccd",
                         "IZSPIOI4D7CJZMNBZEBD",
                         "IZSPIOI4D7CJZMNBZEBD",
                         "video1",
                         "apical",
                         "rtspPort,554",
                         554, 0, 0,
                         response,
                         sizeof(response) - 1);
    printf("ret = %d\n", ret);
    printf("response:\n%s\n", response);

    curl_global_cleanup();
    return 0;
}
#endif


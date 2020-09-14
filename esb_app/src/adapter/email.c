/**
 * @file email.c
 * @author MubeenS
 * @brief The program sends an email
 * to the destination specified. Reads
 * the content from a file specified.
 * 
 * returns NULL on success.
 * @version 0.1
 * @date 2020-09-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdio.h>
#include <string.h>

#include <curl/curl.h>

#include "transport.h"

#define FROM "testmailtm02@gmail.com"
//#define TO "testmailtm02@gmail.com"
#define CC "testmailtm02@gmail.com"

struct upload_status
{
    int lines_read;
};
#if 0
static const char *payload_file_path[] = {
    "Date: Mon, 29 Nov 2010 21:54:29 +1100\r\n",
    "To: " TO "\r\n",
    "From: " FROM " (Example User)\r\n",
    "Cc: " CC " (Another example )\r\n",
    "Message-ID: <dc7cb-11db-487a-9f3a-e52a9458ef@"
    "rfcpedant.example.org>\r\n",
    "Subject: SMTP TLS example message\r\n",
    "\r\n", /* empty line to divide headers from body, see RFC5322 */
    "The body of the message starts here.\r\n",
    "\r\n",
    "It could be a lot of lines, could be MIME encoded, whatever.\r\n",
    "Check RFC5322.\r\n",
    NULL};

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
    struct upload_status *upload_ctx = (struct upload_status *)userp;
    const char *data;

    if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1))
    {
        return 0;
    }

    data = payload_file_path[upload_ctx->lines_read];

    if (data)
    {
        size_t len = strlen(data);
        memcpy(ptr, data, len);
        upload_ctx->lines_read++;

        return len;
    }

    return 0;
}
#endif

void* send_mail(void *toptr, void *file_path_ptr)
{   
    char *to = (char*)toptr;
    char *file_path = (char*)file_path_ptr;
    printf("Sending mail to %s\n", to);

    CURL *curl;
    CURLcode res = CURLE_OK;
    struct curl_slist *recipients = NULL;
    struct upload_status upload_ctx;

    upload_ctx.lines_read = 0;

    curl = curl_easy_init();
    if (curl)
    {
        /* Set username and password */
        curl_easy_setopt(curl, CURLOPT_USERNAME, "testmailtm02");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "great1.2");

        /* URL for mail server */
        curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587/");

        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

        /** TODO: Security certificate to be added 
         * for secure transport protocol 
         */

        //curl_easy_setopt(curl, CURLOPT_CAINFO, "/path/to/certificate.pem");

        /* FROM address */
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM);

        /* Recipients */
        recipients = curl_slist_append(recipients, to);
        recipients = curl_slist_append(recipients, CC);
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        /* We're using a callback function to specify the payload (the headers and
         * body of the message). You could just use the CURLOPT_READDATA option to
         * specify a FILE pointer to read from. */
        //curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        FILE *fp = fopen(file_path, "r");
        curl_easy_setopt(curl, CURLOPT_READDATA, fp);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        /* Gives information about whats running inside libcurl */
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* Send the message */
        /* res =0 means everything is ok */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* Free the list of recipients */
        curl_slist_free_all(recipients);

        /* Always cleanup */
        curl_easy_cleanup(curl);
    }

    return NULL;
}

/*int main()
{
    int t = send_mail("testmailtm02@gmail.com", "../assets/ToBeMailed.json");
    printf("%d", t);
    return 0;
}*/
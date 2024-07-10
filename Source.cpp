#include <iostream>
#include <string>
#include <curl/curl.h>
#include <htmlcxx/html/ParserDom.h>

using namespace std;
using namespace htmlcxx;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    }
    catch (std::bad_alloc& e) {
        // Handle memory problem
        return 0;
    }
    return newLength;
}

string getHTML(const string& url) {
    CURL* curl;
    CURLcode res;
    string html;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return html;
}

void parseHTML(const string& html) {
    HTML::ParserDom parser;
    tree<HTML::Node> dom = parser.parseTree(html);

    // Extracting Profile Name
    auto it = dom.begin();
    for (; it != dom.end(); ++it) {
        if (it->tagName() == "span" && it->attribute("class").second == "p-name vcard-fullname d-block overflow-hidden") {
            cout << "Name: " << it->text() << endl;
            break;
        }
    }

    // Extracting Username
    it = dom.begin();
    for (; it != dom.end(); ++it) {
        if (it->tagName() == "span" && it->attribute("class").second == "p-nickname vcard-username d-block") {
            cout << "Username: " << it->text() << endl;
            break;
        }
    }

    // Extracting Bio
    it = dom.begin();
    for (; it != dom.end(); ++it) {
        if (it->tagName() == "div" && it->attribute("class").second == "p-note user-profile-bio mb-3 js-user-profile-bio f4") {
            cout << "Bio: " << it->text() << endl;
            break;
        }
    }

    // Extracting Location
    it = dom.begin();
    for (; it != dom.end(); ++it) {
        if (it->tagName() == "li" && it->attribute("class").second == "vcard-detail pt-1 css-truncate css-truncate-target") {
            cout << "Location: " << it->text() << endl;
            break;
        }
    }

    // Extracting Repositories count
    it = dom.begin();
    for (; it != dom.end(); ++it) {
        if (it->tagName() == "span" && it->attribute("class").second == "Counter") {
            cout << "Repositories: " << it->text() << endl;
            break;
        }
    }
}

int main() {
    string url = "https://github.com/Nelzouki22";
    string html = getHTML(url);

    if (!html.empty()) {
        parseHTML(html);
    }
    else {
        cout << "Failed to retrieve the profile page." << endl;
    }

    return 0;
}

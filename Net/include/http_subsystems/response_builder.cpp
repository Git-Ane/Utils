#include "response_builder.hpp"
namespace GitAne::Net{
    std::string buildOkPage(std::string title, std::string body){
        /*
        \brief take title & body and return a webpage to send as a reponse with the error Ok.
        */
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1>" + title +  "</h1>" + body + "</body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
        << htmlFile;
        std::string s = ss.str();
        return s;
    }
    std::string buildCreatedPage(std::string title, std::string body){
        /*
        \brief take title & body and return a webpage to send as a reponse with the error Created.
        */
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1>" + title +  "</h1>" + body + "</body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 201 Created\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
        << htmlFile;
        std::string s = ss.str();
        return s;
    }
    std::string buildAcceptedPage(std::string title, std::string body){
        /*
        \brief take title & body and return a webpage to send as a reponse with the error Accepted.
        */
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1>" + title +  "</h1>" + body + "</body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 202 Created\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
        << htmlFile;
        std::string s = ss.str();
        return s;
    }
    std::string buildBadRequestPage(std::string title, std::string body){
        /*
        \brief take title & body and return a webpage to send as a reponse with the error Bad.
        */
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1>" + title +  "</h1>" + body + "</body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 400 Bad Request\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
        << htmlFile;
        std::string s = ss.str();
        return s;
    }

    std::string buildUnauthorizedRequestPage(std::string title, std::string body){
        /*
        \brief take title & body and return a webpage to send as a reponse with the error Unauthorized.
        */
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1>" + title +  "</h1>" + body + "</body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 401 Unauthorized\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
        << htmlFile;
        std::string s = ss.str();
        return s;
    }

    std::string buildNotFoundPage(std::string title, std::string body){
        /*
        \brief take title & body and return a webpage to send as a reponse with the error NotFound.
        */
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1>" + title +  "</h1>" + body + "</body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
        << htmlFile;
        std::string s = ss.str();
        return s;
    }

    std::string buildInternalServerErrorPage(std::string title, std::string body){
        /*
        \brief take title & body and return a webpage to send as a reponse with the error InteralServerError.
        */
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1>" + title +  "</h1>" + body + "</body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 500 Internal Server Error\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
        << htmlFile;
        std::string s = ss.str();
        return s;
    }

    std::string buildNotImplementedPage(std::string title, std::string body){
        /*
        \brief take title & body and return a webpage to send as a reponse with the error NotImplemented.
        */
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1>" + title +  "</h1>" + body + "</body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 501  Not Implemented\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
        << htmlFile;
        std::string s = ss.str();
        return s;
    }
}
// url_tree.cpp
// For GitÂne's Net
#include "url_tree.hpp"


// ce fichier tu as pas à le modifier.

void Node::addChild(const std::string& urlPart, const Node& child) {
    children[urlPart] = child;
}

void Node::addLeaf(const std::string& name, std::function<std::string(std::string method, std::string args)> leafAction) {
    Node leaf = Node();
    leaf.action = leafAction;
    children[name] = leaf;
}

void UrlTree::addPath(const std::string& path, std::function<std::string(std::string method, std::string args)> action) {
    /*
    \brief ajoute action comme réponse à l'url path
    \param path
    \param la fonction d'action
    */
    std::cout << "Ajout de " << path << std::endl;
    Node* currentNode = &root;
    size_t start = 0;
    size_t end = path.find('/');

    while (end != std::string::npos) {
        
        const std::string& urlPart = path.substr(start, end - start);
        std::cout << "Sous-partie gérée: " << urlPart << std::endl;
        currentNode = &(currentNode->children[urlPart]);
        start = end + 1;
        end = path.find('/', start);
    }

    const std::string& lastPart = path.substr(start);
    std::cout << "Sous-partie gérée: " << lastPart << std::endl;
    currentNode->addLeaf(lastPart, action);
}

void UrlTree::printTreeHelper(const Node& node, const std::string& currentPath) const {
    if (!node.children.empty()) {
        for (const auto& [part, child] : node.children) {
            printTreeHelper(child, currentPath + part + "/");
        }
    }

    if (node.action) {
        std::cout << "Path: " << currentPath << " Action: [Defined]" << std::endl;
    }
    else{
        std::cout << "Path: " << currentPath << "n'a pas d'action." << std::endl;
    }
}

void UrlTree::printTree() const {
    printTreeHelper(root, "");
}

std::function<std::string(std::string method, std::string args)> UrlTree::getActionForUrl(const std::string& url) const {

    printTree();
    std::cout << "Début Get Action pour " << url << std::endl;
    const Node* currentNode = &root;
    size_t start = 0;
    size_t end = url.find('/');

    while (end != std::string::npos) {
        const std::string& urlPart = url.substr(start, end - start);
        std::cout << "Se déplace dans "<< urlPart << std::endl;
        auto it = currentNode->children.find(urlPart);
        if (it == currentNode->children.end()) {
            // Le chemin n'existe pas
            return nullptr;
        }

        currentNode = &(it->second);
        start = end + 1;
        end = url.find('/', start);
    }

    return currentNode->action;

    const std::string& lastPart = url.substr(start);
     std::cout << "Se déplace dans "<< lastPart << std::endl;
    auto leafIt = currentNode->children.find(lastPart);
    if (leafIt != currentNode->children.end()) {
        std::cout <<  "Bonne Fin Get Action\n";

        return leafIt->second.action;
    }
    
    std::cout <<  "Mauvaise Fin Get Action\n";
    return nullptr;  // La feuille n'a pas d'action associée
}   

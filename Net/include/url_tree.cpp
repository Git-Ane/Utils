// url_tree.cpp
// For GitÂne's Net
#include "url_tree.hpp"

void Node::addChild(const std::string& urlPart, const Node& child) {
    children[urlPart] = child;
}

void Node::addLeaf(const std::string& name, std::function<std::string()> leafAction) {
    Node leaf = Node();
    leaf.action = leafAction;
    children[name] = leaf;
}

void UrlTree::addPath(const std::string& path, std::function<std::string()> action) {
    Node* currentNode = &root;

    size_t start = 0;
    size_t end = path.find('/');

    while (end != std::string::npos) {
        const std::string& urlPart = path.substr(start, end - start);
        currentNode = &(currentNode->children[urlPart]);
        start = end + 1;
        end = path.find('/', start);
    }

    const std::string& lastPart = path.substr(start);
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
}

void UrlTree::printTree() const {
    printTreeHelper(root, "");
}

std::function<std::string()> UrlTree::getActionForUrl(const std::string& url) const {
    const Node* currentNode = &root;

    size_t start = 0;
    size_t end = url.find('/');

    while (end != std::string::npos) {
        const std::string& urlPart = url.substr(start, end - start);
        auto it = currentNode->children.find(urlPart);
        if (it == currentNode->children.end()) {
            // Le chemin n'existe pas
            return nullptr;
        }

        currentNode = &(it->second);
        start = end + 1;
        end = url.find('/', start);
    }

    const std::string& lastPart = url.substr(start);
    auto leafIt = currentNode->children.find(lastPart);
    if (leafIt != currentNode->children.end()) {
        return leafIt->second.action;
    }

    return nullptr;  // La feuille n'a pas d'action associée
}   

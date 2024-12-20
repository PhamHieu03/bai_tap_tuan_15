#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// Định nghĩa loại của node
enum class NodeType {
    Chapter,
    Section,
    Subsection,
    Subsubsection
};

// Cấu trúc của một node trong cây mục lục
struct TOCNode {
    std::string title;               // Tên mục
    NodeType type;                   // Loại mục (Chapter, Section, ...)
    int startPage;                   // Trang bắt đầu
    int pageCount;                   // Số trang của mục
    std::vector<TOCNode*> children;  // Các mục con

    TOCNode(std::string t, NodeType ty, int start, int count)
        : title(t), type(ty), startPage(start), pageCount(count) {}
};
int countChapters(TOCNode* root) {
    if (!root) return 0;
    int count = 0;
    if (root->type == NodeType::Chapter) {
        count = 1;
    }
    for (auto child : root->children) {
        count += countChapters(child);
    }
    return count;
}
TOCNode* findLongestChapter(TOCNode* root) {
    if (!root) return nullptr;
    TOCNode* longest = nullptr;

    // Nếu node hiện tại là chương, kiểm tra
    if (root->type == NodeType::Chapter) {
        longest = root;
    }

    for (auto child : root->children) {
        TOCNode* temp = findLongestChapter(child);
        if (temp) {
            if (!longest || temp->pageCount > longest->pageCount) {
                longest = temp;
            }
        }
    }
    return longest;
}
bool deleteItem(TOCNode* parent, const std::string& title) {
    if (!parent) return false;

    // Tìm mục cần xoá trong danh sách children của parent
    auto it = std::find_if(parent->children.begin(), parent->children.end(),
        [&](TOCNode* node) { return node->title == title; });

    if (it != parent->children.end()) {
        // Xoá mục
        TOCNode* toDelete = *it;
        parent->children.erase(it);

        // Giải phóng bộ nhớ nếu cần
        // delete toDelete; // Nếu sử dụng con trỏ động

        // Cập nhật số trang cho các mục sau
        int pagesToRemove = toDelete->pageCount;
        for (auto& sibling : parent->children) {
            if (sibling->startPage > toDelete->startPage) {
                sibling->startPage -= pagesToRemove;
            }
        }
        return true;
    }

    // Nếu không tìm thấy trong children, tìm đệ quy trong các children
    for (auto child : parent->children) {
        if (deleteItem(child, title)) {
            return true;
        }
    }

    return false; // Không tìm thấy mục cần xoá
}
int main() {
    // Tạo cây mục lục
    TOCNode* root = new TOCNode("Sách Giáo Trình", NodeType::Chapter, 1, 100);

    // Thêm chương 1
    TOCNode* chapter1 = new TOCNode("Chương 1: Giới Thiệu", NodeType::Chapter, 1, 30);
    root->children.push_back(chapter1);

    // Thêm chương 2
    TOCNode* chapter2 = new TOCNode("Chương 2: Cơ Bản", NodeType::Chapter, 31, 50);
    root->children.push_back(chapter2);

    // Thêm mục trong chương 1
    TOCNode* section1 = new TOCNode("Mục 1.1: Khái Niệm", NodeType::Section, 1, 15);
    chapter1->children.push_back(section1);

    TOCNode* section2 = new TOCNode("Mục 1.2: Ứng Dụng", NodeType::Section, 16, 15);
    chapter1->children.push_back(section2);

    // Đếm số chương
    int numChapters = countChapters(root);
    std::cout << "Số chương trong sách: " << numChapters << std::endl;

    // Tìm chương dài nhất
    TOCNode* longestChapter = findLongestChapter(root);
    if (longestChapter) {
        std::cout << "Chương dài nhất: " << longestChapter->title
            << " với " << longestChapter->pageCount << " trang." << std::endl;
    }

    // Xoá mục 1.2 và cập nhật số trang
    bool deleted = deleteItem(root, "Mục 1.2: Ứng Dụng");
    if (deleted) {
        std::cout << "Đã xoá mục 'Mục 1.2: Ứng Dụng' và cập nhật số trang." << std::endl;
    }
    else {
        std::cout << "Không tìm thấy mục cần xoá." << std::endl;
    }

    // Giải phóng bộ nhớ
    // (Cần duyệt và xoá toàn bộ cây nếu sử dụng con trỏ động)

    return 0;
}

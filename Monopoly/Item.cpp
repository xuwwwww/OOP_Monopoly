#include "Item.h"
#include "Player.h"
#include <conio.h>
#include <iostream>
#include <string>

void ControlDiceItem::Use(Player* p)
{
    // 控制骰子：讓玩家選擇擲骰子點數的效果
    std::cout << "請選擇要前進的步數 (1-6)：\n";
    
    int selected = 1;
    bool confirmed = false;
    
    // 使用方向鍵上下選擇要走的步數
    while (!confirmed) {
        std::cout << "\033[2J\033[H"; // 清空螢幕
        std::cout << "控制骰子：請選擇要前進的步數\n\n";
        
        // 顯示選項
        for (int i = 1; i <= 6; ++i) {
            if (i == selected) {
                std::cout << " > " << "【" << i << "】" << "\n";
            } else {
                std::cout << "   " << "【" << i << "】" << "\n";
            }
        }
        
        // 接收使用者輸入
        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) selected = (selected + 4) % 6 + 1; // 上
            if (key == 80) selected = selected % 6 + 1; // 下
        }
        else if (key == '\r') { // Enter 鍵確認
            confirmed = true;
        }
    }
    
    std::cout << "你選擇了前進 " << selected << " 步！\n";
    
    // 取得地圖大小（此處假設為28格，實際上應取自 Game 類別或相關邏輯）
    // 在真實場景中，這個值應該從遊戲邏輯中獲取
    const int mapSize = 28;
    
    // 移動玩家
    p->Move(selected, mapSize);
}

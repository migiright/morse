#pragma once

void button_init();
int button_pushed(); //チャタリング対応処理したあとの仮想的な状態
int button_pushedLaw(); //GPIO_ReadInputDataBitで読んだ値そのもの

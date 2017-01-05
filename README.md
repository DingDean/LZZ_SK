# 对杀双扣/千变双扣机器人逻辑

## 场景：
* 接牌: 其他玩家出牌后，轮到自己出牌
* 出牌: 自己争取到的出牌机会

### 接牌:
判断是否有能力接牌。若是，看有多少种选择，择其优者出之。否则，过牌
### 出牌：
获得可以出牌的选择，择其优者出之。
### 量化一个选择的优劣
有以下指标:
1. 能否最大化牌局得分
2. 能否获得最大的连续出牌次数:
    * 
3. 能否最小化散牌数量
4. 

## 代码风格
* 依照Google C++ Style Guide
* 使用Catch来进行单元测试





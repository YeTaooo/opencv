https://blog.csdn.net/guyuealian/article/details/82660826
这个实现很好 将space weight 和color weight 先单独计算出来

kernel 就是以一个pixel为中心 半径为r 的方阵 
更新kernel点的pixel 即以kernel周围点的距离和像素差为权重重新计算

gaussian blur 仅仅考虑距离权重未考虑像素 所以没有preserve edge
而bilateral filter 既考虑距离又考虑像素 可以达到保留边缘的目的 因为像素相差较大的邻居像素的权重值很小


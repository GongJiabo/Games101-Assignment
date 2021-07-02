### 代码问题

1. shadowPointOrig的计算需要在发向量上进行偏移？
    这是因为表面的有些点由于损失精度，到了表面内部
Vector3f shadowPointOrig = (dotProduct(dir, N) < 0) ? hitPoint + N * scene.epsilon : hitPoint - N * scene.epsilon;

2. 计算着色点光强light->intensity没有除以光源到着色点的距离平方？

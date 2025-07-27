### 【0338】ask perpendicular 2D 向量的二维正交向量

#### 代码

```cpp
    /* UF_VEC2_ask_perpendicular may return either the positive or negative  
       perpendicular vector.  This function will always return the positive one.  
    */  
    static void ask_perpendicular_2D(double vec[2], double perp_vec[2])  
    {  
        y[0] =  vec[1];  
        y[1] = -vec[0];  
    }

```

#### 代码解析

> 这段代码实现了在二维空间中找到一个向量vec的正交向量perp_vec。它首先将vec的y分量赋值给perp_vec的x分量，然后将vec的x分量取反后赋值给perp_vec的y分量。这样perp_vec与vec正交，且总是指向vec的顺时针方向。具体来说：
>
> 1. perp_vec[0] = vec[1]：将vec的y分量赋值给perp_vec的x分量。
> 2. perp_vec[1] = -vec[0]：将vec的x分量取反后赋值给perp_vec的y分量。
>
> 通过这个操作，perp_vec与vec垂直，并且总是指向vec的顺时针方向。这个函数对于二维图形处理，比如计算法线向量等场景非常有用。
>

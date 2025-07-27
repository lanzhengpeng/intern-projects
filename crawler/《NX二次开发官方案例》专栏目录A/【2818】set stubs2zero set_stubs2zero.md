### 【2818】set stubs2zero set_stubs2zero

#### 代码

```cpp
    /*HEAD SET_STUBS2ZERO CCC UFUN */  
    static void set_stubs2zero(void)  
    {  
        char  
            diameter[27],  
            radius[27];  
        double  
            mpr[70];  
        int  
            mpi[100];  
            UF_CALL(UF_DRF_ask_preferences(mpi, mpr, radius, diameter));  
        mpr[9] = 0.0; /* Stub size = 0 译:Stub size = 0，译文为：存根大小为0。 */  
        UF_CALL(UF_DRF_set_preferences(mpi, mpr, radius, diameter));  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发函数，其功能是设置NX绘图环境中的存根大小为0。
>
> 主要步骤包括：
>
> 1. 定义字符数组diameter、radius和双精度数组mpr、整型数组mpi，用于存储参数。
> 2. 调用UF_DRF_ask_preferences函数，获取当前的绘图参数存入mpi、mpr、radius、diameter数组。
> 3. 将mpr数组的第10个元素（即存根大小参数）设置为0，表示设置存根大小为0。
> 4. 调用UF_DRF_set_preferences函数，将修改后的参数设置回绘图环境。
> 5. 通过设置存根大小为0，可以避免在绘图时出现存根，使绘图更加清晰。
> 6. UF_CALL是错误处理宏，用于在函数调用出错时进行错误处理。
> 7. 函数名set_stubs2zero表示将存根大小设置为0，体现了该函数的主要功能。
>
> 总体来说，这段代码实现了设置NX绘图环境参数的功能，通过修改存根大小参数，避免了绘图时出现存根，提高了绘图质量。
>

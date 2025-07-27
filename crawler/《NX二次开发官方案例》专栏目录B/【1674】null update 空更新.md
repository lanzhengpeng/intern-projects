### 【1674】null update 空更新

#### 代码

```cpp
    static void null_update(tag_t object)  
    {  
        int  
            one = 1,  
            resp,  
            two = 2,  
            zero = 0;  
        double  
            mx[12],  
            zero_delta[3] = { 0, 0, 0 };  
        FTN(uf5943)(zero_delta, mx);  
        FTN(uf5947)(mx, &object, &one, &one, &zero, &two, NULL, NULL, &resp);  
    }

```

#### 代码解析

> 这是段NX Open C++二次开发代码，用于更新NX部件的特征参数。具体来说，代码实现了以下功能：
>
> 1. 定义了静态方法null_update，接收NX部件对象作为参数。
> 2. 在方法内部，定义了一些整型变量用于存储状态码和配置参数，包括one、two和zero。
> 3. 定义了浮点数组mx，用于存储部件的12个方向的最大值。
> 4. 定义了浮点数组zero_delta，用于存储部件的位置偏移量，初始值为{0, 0, 0}。
> 5. 调用了uf5943函数，将位置偏移量zero_delta赋值给mx数组。
> 6. 调用了uf5947函数，更新部件的特征参数，包括位置、方向、颜色等，具体参数通过指针传递。
> 7. uf5947函数的返回值存储在resp变量中，用于判断更新操作是否成功。
> 8. 整个方法没有返回值，是静态方法，可以直接调用。
> 9. uf5943和uf5947是NX Open C++的内置函数，用于与NX内核交互。
> 10. 这段代码实现了简单的部件更新操作，没有涉及复杂的逻辑，属于NX二次开发的入门代码示例。
>

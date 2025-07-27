### 【1288】find next empty layer 找到下一个空白的图层

#### 代码

```cpp
    static int find_next_empty_layer(int start)  
    {  
        int  
            ii;  
        tag_t  
            object;  
        for (ii = start; ii < 257; ii++)  
        {  
            object = NULL_TAG;  
            UF_CALL(UF_LAYER_cycle_by_layer(ii, &object));  
            if (object == NULL_TAG) return ii;  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是用于在NX软件中查找下一个空白的图层的函数。以下是代码的主要功能：
>
> 1. 函数名为find_next_empty_layer，接受一个整数参数start，表示从哪个图层编号开始查找。
> 2. 定义了变量ii用于遍历图层编号，object用于存储UF_CALL返回的对象标签。
> 3. 使用for循环从start开始遍历到256号图层。
> 4. 在循环体内，使用UF_CALL调用UF_LAYER_cycle_by_layer函数，传入当前遍历的图层编号ii，以及对象标签的地址&object。
> 5. UF_CALL会返回当前图层上的对象，如果当前图层为空，则返回NULL_TAG。
> 6. 如果返回的对象为NULL_TAG，则表示当前图层为空，函数返回当前遍历到的图层编号ii。
> 7. 如果循环结束后还没有找到空图层，则返回0表示没有空图层。
>
> 总之，这段代码的作用是遍历从指定编号开始的图层，直到找到第一个空图层，并返回该图层的编号。如果没有空图层，则返回0。
>

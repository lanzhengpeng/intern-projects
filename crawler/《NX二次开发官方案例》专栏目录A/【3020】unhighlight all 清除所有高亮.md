### 【3020】unhighlight all 清除所有高亮

#### 代码

```cpp
    #include <uf_part.h>  
    static void unhighlight_all(void)  
    {  
        UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要功能是调用UF_PART_cleanup函数来清除所有高亮显示的零件。具体来说，该函数接受一个指向UF_PART_cleanup_highlight函数的指针作为参数。在代码中，首先包含头文件uf_part.h，该文件包含UF_PART_cleanup函数的声明。然后定义了一个名为unhighlight_all的静态函数，该函数内部调用了UF_PART_cleanup函数来清除所有高亮显示的零件。最后，调用unhighlight_all函数来执行清除高亮显示的操作。这段代码的主要作用是提供一个API接口，让其他代码可以调用以清除零件的高亮显示。
>

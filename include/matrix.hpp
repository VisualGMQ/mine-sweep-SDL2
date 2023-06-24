#pragma once

template <typename T>
class Matrix final {
public:
    Matrix(int w, int h): data_(std::unique_ptr<T[]>(new T[w * h])), w_(w), h_(h) {}

    void Fill(const T& value) {
        for (int i = 0 ; i < w_ * h_; i++) {
            GetByIndex(i) = value;
        }
    }

    T& Get(int x, int y) {
        return data_.get()[x + y * w_];
    }

    const T& Get(int x, int y) const {
        return data_.get()[x + y * w_];
    }

    T& GetByIndex(int idx) {
        return data_.get()[idx];
    }

    int MaxSize() const {
        return w_ * h_;
    }

    int Width() const {
        return w_;
    }

    int Height() const {
        return h_;
    }

    bool IsIn(int x, int y) const {
        return x >= 0 && x < w_ && y >= 0 && y < h_;
    }

private:
    std::unique_ptr<T[]> data_;
    int w_;
    int h_;
};
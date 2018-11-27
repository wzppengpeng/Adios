#ifndef WZP_BIG_DATA_BITMAP_HPP_
#define WZP_BIG_DATA_BITMAP_HPP_

#include <vector>

struct BitMap {
  /*! \brief internal data structure */
  std::vector<uint32_t> data;
  /*!
   * \brief resize the bitmap to be certain size
   * \param size the size of bitmap
   */
  inline void Resize(size_t size) {
    data.resize((size + 31U) >> 5, 0);
  }
  /*!
   * \brief query the i-th position of bitmap
   * \param i the position in
   */
  inline bool Get(size_t i) const {
    return (data[i >> 5] >> (i & 31U)) & 1U;
  }
  /*!
   * \brief set i-th position to true
   * \param i position index
   */
  inline void SetTrue(size_t i) {
    data[i >> 5] |= (1 << (i & 31U));
  }
  /*! \brief initialize the value of bit map from vector of bool*/
  inline void InitFromBool(const std::vector<int>& vec) {
    this->Resize(vec.size());
    // parallel over the full cases
    auto nsize = static_cast<unsigned>(vec.size() / 32);
    for (unsigned i = 0; i < nsize; ++i) {
      uint32_t res = 0;
      for (int k = 0; k < 32; ++k) {
        int bit = vec[(i << 5) | k];
        res |= (bit << k);
      }
      data[i] = res;
    }
    if (nsize != vec.size()) data.back() = 0;
    for (size_t i = nsize; i < vec.size(); ++i) {
      if (vec[i]) this->SetTrue(i);
    }
  }
  /*! \brief clear the bitmap, set all places to false */
  inline void Clear() {
    std::fill(data.begin(), data.end(), 0U);
  }
};

#endif /*WZP_BIG_DATA_BITMAP_HPP_*/
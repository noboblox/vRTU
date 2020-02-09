#ifndef IEC104_SMARTPOINTER_HPP_
#define IEC104_SMARTPOINTER_HPP_

extern "C"
{
#include "iec60870_common.h"
#include "cs101_information_objects.h"
}

namespace IEC104
{
  template <typename PtrT>
  class SpInfoObjectPointer
  {
  public:
    SpInfoObjectPointer() : mpHandle(nullptr) {}

    explicit SpInfoObjectPointer(PtrT apOwned)
      : mpHandle(apOwned) {}

    SpInfoObjectPointer(const SpInfoObjectPointer&) = delete;
    SpInfoObjectPointer& operator=(const SpInfoObjectPointer&) = delete;

    SpInfoObjectPointer(SpInfoObjectPointer&& arOther)
      : mpHandle(arOther.Release()) {}

    SpInfoObjectPointer& operator=(SpInfoObjectPointer&& arOther)
    {
      Reset(nullptr);
      mpHandle = arOther.Release();
      return *this;
    }

    void Reset(PtrT apNewOwned) noexcept
    {
      if (mpHandle)
      {
        InformationObject pBase = reinterpret_cast<InformationObject>(mpHandle);
        InformationObject_destroy(pBase);
      }
      mpHandle = apNewOwned;
    }

    PtrT Get() const noexcept
    {
      return mpHandle;
    }

    PtrT Release() noexcept
    {
      PtrT pUnowned = mpHandle;
      mpHandle = nullptr;
      return pUnowned;
    }

    ~SpInfoObjectPointer()
    {
      Reset(nullptr);
    }

  private:
    PtrT mpHandle;
  };
}
#endif

#pragma once

#include "../dxvk/dxvk_device.h"

#include "d3d11_device_child.h"

namespace dxvk {
  
  class D3D11Device;
  
  /**
   * \brief Generic resource view template
   * 
   * Stores an image view or a buffer view, depending
   * on the referenced resource type, and implements
   * the interface for a given view type.
   * \tparam Iface Base interface
   * \tparam DescType View description type
   */
  template<typename Iface, typename DescType>
  class D3D11ResourceView : public D3D11DeviceChild<Iface> {
    
  public:
    
    D3D11ResourceView(
            D3D11Device*                      device,
            ID3D11Resource*                   resource,
      const DescType&                         desc,
      const Rc<DxvkBufferView>&               bufferView)
    : m_device(device), m_resource(resource),
      m_desc(desc), m_bufferView(bufferView) { }
    
    D3D11ResourceView(
            D3D11Device*                      device,
            ID3D11Resource*                   resource,
      const DescType&                         desc,
      const Rc<DxvkImageView>&                imageView)
    : m_device(device), m_resource(resource),
      m_desc(desc), m_imageView(imageView) { }
    
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) final {
      COM_QUERY_IFACE(riid, ppvObject, IUnknown);
      COM_QUERY_IFACE(riid, ppvObject, ID3D11DeviceChild);
      COM_QUERY_IFACE(riid, ppvObject, ID3D11View);
      COM_QUERY_IFACE(riid, ppvObject, Iface);
      
      Logger::warn("D3D11ResourceView::QueryInterface: Unknown interface query");
      return E_NOINTERFACE;
    }
    
    void STDMETHODCALLTYPE GetDevice(ID3D11Device** ppDevice) final {
      *ppDevice = m_device.ref();
    }
    
    void STDMETHODCALLTYPE GetResource(ID3D11Resource** ppResource) final {
      *ppResource = m_resource.ref();
    }
    
    void STDMETHODCALLTYPE GetDesc(DescType* pDesc) final {
      *pDesc = m_desc;
    }
    
    D3D11_RESOURCE_DIMENSION GetResourceType() const {
      D3D11_RESOURCE_DIMENSION type;
      m_resource->GetType(&type);
      return type;
    }
    
    Rc<DxvkBufferView> GetBufferView() const {
      return m_bufferView;
    }
    
    Rc<DxvkImageView> GetImageView() const {
      return m_imageView;
    }
    
  private:
    
    Com<D3D11Device>    m_device;
    Com<ID3D11Resource> m_resource;
    DescType            m_desc;
    Rc<DxvkBufferView>  m_bufferView;
    Rc<DxvkImageView>   m_imageView;
    
  };
  
  
  using D3D11ShaderResourceView = D3D11ResourceView<
    ID3D11ShaderResourceView, D3D11_SHADER_RESOURCE_VIEW_DESC>;
  
  using D3D11RenderTargetView = D3D11ResourceView<
    ID3D11RenderTargetView, D3D11_RENDER_TARGET_VIEW_DESC>;
  
  using D3D11DepthStencilView = D3D11ResourceView<
    ID3D11DepthStencilView, D3D11_DEPTH_STENCIL_VIEW_DESC>;
  
}

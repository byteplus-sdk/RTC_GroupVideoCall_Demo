
Pod::Spec.new do |spec|
  spec.name         = 'GroupVideoCallDemo'
  spec.version      = '1.0.0'
  spec.summary      = 'GroupVideoCallDemo APP'
  spec.description  = 'GroupVideoCallDemo App Demo..'
  spec.homepage     = 'https://github.com/byteplus-sdk'
  spec.license      = { :type => 'MIT', :file => 'LICENSE' }
  spec.author       = { 'author' => 'byteplus rtc' }
  spec.source       = { :path => './'}
  spec.ios.deployment_target = '9.0'
  
  spec.source_files = '**/*.{h,m,c,mm}'
  spec.resource_bundles = {
    'GroupVideoCallDemo' => ['Resource/*.{xcassets,bundle}']
  }
  spec.pod_target_xcconfig = {'CODE_SIGN_IDENTITY' => ''}
  spec.prefix_header_contents = '#import "Masonry.h"',
                                '#import "ToolKit.h"',
                                '#import "GroupVideoCallDemoConstants.h"'
  spec.dependency 'ToolKit'
  spec.dependency 'YYModel'
  spec.dependency 'Masonry'
end

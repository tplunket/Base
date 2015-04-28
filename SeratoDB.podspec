Pod::Spec.new do |s|

  s.name         = "SeratoDB"
  s.version      = "0.11"
  s.summary      = "A reader/writer for Serato track collections."

  s.description  = <<-DESC
                   This library contains confidential and proprietary information of Serato Inc. LLP ("Serato"). No use is permitted without express written permission of Serato.

                   If you are not a party to a Confidentiality/Non-Disclosure Agreement with Serato, please immediately delete this library as well as all copies in your possession.

                   For further information, please refer to the modified MIT license provided with this library, or email licensing@serato.com.
                   DESC

  s.homepage     = "http://github.com/NextAudioLabs"

  s.license      = "Modified MIT"

  s.authors      = { "Didier Malenfant" => "didier@nextaudiolabs.com" }

  s.platform     = :osx, "10.7"

  s.osx.deployment_target = "10.7"
  
  s.requires_arc = false

  s.source       = { :git => "https://github.com/NextAudioLabs/SeratoDB.git", :tag => "v#{s.version}" }

  s.dependency 'taglib', '~> 1.9.1-4'

  s.compiler_flags = '-std=c++1y'

  s.default_subspec = 'Core'
  
  s.subspec "Core" do |sp|
    sp.source_files = "SeratoDB/*.{h,hpp,cpp}"
  end

  s.subspec "Tests" do |sp|
    sp.source_files = "SeratoDB/Tests/*.{h,hpp,cpp}"
  end
end

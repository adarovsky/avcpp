#
# Be sure to run `pod lib lint avcpp.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
  s.name             = 'avcpp'
  s.version          = '1.0.0'
  s.summary          = 'Wrapper for the FFmpeg that simplify usage it from C++ projects.'

# This description is used to generate tags and improve search results.
#   * Think: What does it do? Why did you write it? What is the focus?
#   * Try to keep it short, snappy and to the point.
#   * Write the description between the DESC delimiters below.
#   * Finally, don't worry about the indent, CocoaPods strips it!

  s.description      = <<-DESC
  Currently covered next functionality:
    - Core helper & utility classes (AVFrame -> av::AudioSample & av::VideoFrame, AVRational -> av::Rational and so on)
    - Container formats & contexts muxing and demuxing
    - Codecs & codecs contexts: encoding and decoding
    - Streams (AVStream -> av::Stream)
    - Filters (audio & video): parsing from string, manual adding filters to the graph & other
    - SW Video & Audio resamplers
                       DESC

  s.homepage         = 'https://bitbucket.org/vyulabs'
  # s.screenshots     = 'www.example.com/screenshots_1', 'www.example.com/screenshots_2'
  s.license          = { :type => 'BSD', :file => 'LICENSE-bsd.txt' }
  s.author           = { 'h4tr3d' => 'h4tr3d@github.com' }
  s.source           = { :git => 'https://github.com/adarovsky/avcpp.git', :tag => s.version.to_s }
  # s.social_media_url = 'https://twitter.com/<TWITTER_USERNAME>'

  s.osx.deployment_target = '10.12'

  s.source_files = 'src/**/*{h,cpp}'
  s.public_header_files = 'src/**/*.h'

  # s.resource_bundles = {
  #   'avcpp' => ['avcpp/Assets/*.png']
  # }

  # s.public_header_files = 'Pod/Classes/**/*.h'
  # s.frameworks = 'UIKit', 'MapKit'
  s.dependency 'ffmpeg'
end

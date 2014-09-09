#include "pch.h"

using namespace concurrency;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Nokia::Graphics::Imaging;
using namespace Platform;
using namespace Platform::Collections;
using namespace VideoEffects;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Media;
using namespace Windows::Media::MediaProperties;
using namespace Windows::Media::Transcoding;
using namespace Windows::Storage;

// Note: this does not work - ExampleFilterChain must be in a separate WinRT DLL
//namespace UnitTests
//{
//    public ref class ExampleFilterChain sealed : IFilterChainFactory
//    {
//    public:
//        virtual IIterable<IFilter^>^ Create()
//        {
//            auto filters = ref new Vector<IFilter^>();
//            filters->Append(ref new AntiqueFilter());
//            filters->Append(ref new FlipFilter(FlipMode::Horizontal));
//            return filters;
//        }
//    };
//}
//using namespace UnitTests;

TEST_CLASS(MediaTranscoderTests)
{
public:
    TEST_METHOD(CX_W_MT_Basic)
    {
        StorageFile^ source = Await(StorageFile::GetFileFromApplicationUriAsync(ref new Uri(L"ms-appx:///Input/Car.mp4")));
        StorageFile^ destination = Await(KnownFolders::VideosLibrary->CreateFileAsync(L"CX_W_MT_Basic.mp4", CreationCollisionOption::ReplaceExisting));

        // Note: this does not work - ExampleFilterChain must be in a separate WinRT DLL
        // auto definition = ref new LumiaEffectDefinition(ExampleFilterChain().GetType()->FullName);

        auto definition = ref new LumiaEffectDefinition(ref new FilterChainFactory([]()
        {
            auto filters = ref new Vector<IFilter^>();
            filters->Append(ref new AntiqueFilter());
            filters->Append(ref new FlipFilter(FlipMode::Horizontal));
            return filters;
        }));

        auto transcoder = ref new MediaTranscoder();
        transcoder->AddVideoEffect(definition->ActivatableClassId, true, definition->Properties);

        PrepareTranscodeResult^ transcode = Await(transcoder->PrepareFileTranscodeAsync(source, destination, MediaEncodingProfile::CreateMp4(VideoEncodingQuality::Qvga)));
        Await(transcode->TranscodeAsync());
    }
};
#include <windows.h>
#include <shlobj.h>

#pragma region
// https://devblogs.microsoft.com/oldnewthing/20080528-00/?p=22163
class ProcessReference : public IUnknown {
public:
	STDMETHODIMP QueryInterface(REFIID riid, void** ppv)
	{
		if (riid == IID_IUnknown) {
			*ppv = static_cast<IUnknown*>(this);
			AddRef();
			return S_OK;
		}
		*ppv = NULL; return E_NOINTERFACE;
	}

	STDMETHODIMP_(ULONG) AddRef()
	{
		return InterlockedIncrement(&m_cRef);
	}

	STDMETHODIMP_(ULONG) Release()
	{
		LONG lRef = InterlockedDecrement(&m_cRef);
		if (lRef == 0) PostThreadMessage(m_dwThread, WM_NULL, 0, 0);
		return lRef;
	}

	ProcessReference()
		: m_cRef(1), m_dwThread(GetCurrentThreadId())
	{
		SHSetInstanceExplorer(this);
	}

	~ProcessReference()
	{
		SHSetInstanceExplorer(NULL);
		Release();

		MSG msg;
		while (m_cRef && GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

private:
	LONG m_cRef;
	DWORD m_dwThread;
};
#pragma endregion

int wmain(int argc, PWSTR argv[])
{
	ProcessReference ref;
	SHELLEXECUTEINFOW info = {
		.cbSize = sizeof info,
		.fMask = SEE_MASK_INVOKEIDLIST | SEE_MASK_NOCLOSEPROCESS,
		.lpVerb = L"properties",
		.lpFile = argc < 2 ? LR"(C:\Windows\System32\drivers\ntfs.sys)" : argv[1],
		.nShow = SW_SHOW,
	};
	ShellExecuteExW(&info);
	return EXIT_SUCCESS;
}
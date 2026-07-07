
// project1Doc.cpp : implementation of the Cproject1Doc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "project1.h"
#endif

#include "project1Doc.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Cproject1Doc

IMPLEMENT_DYNCREATE(Cproject1Doc, CDocument)

BEGIN_MESSAGE_MAP(Cproject1Doc, CDocument)
END_MESSAGE_MAP()


// Cproject1Doc construction/destruction

Cproject1Doc::Cproject1Doc() noexcept
{
	// TODO: add one-time construction code here

}

Cproject1Doc::~Cproject1Doc()
{
}

BOOL Cproject1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

BOOL Cproject1Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// 1. Clear out previous pixels from RAM if an image is already open
	if (!m_image.IsNull()) {
		m_image.Destroy();
	}

	// 2. Load the new file! Windows decodes your JPG/BMP/PNG/GIF/TIF automatically
	HRESULT hr = m_image.Load(lpszPathName);

	// Clone the original data structure immediately
	if (!m_image.IsNull())
	{
		if (!m_imageOriginal.IsNull()) m_imageOriginal.Destroy();

		// Create an empty canvas with identical dimensions and bit depth
		m_imageOriginal.Create(m_image.GetWidth(), m_image.GetHeight(), m_image.GetBPP());

		// Copy the raw pixel bytes over
		m_image.BitBlt(m_imageOriginal.GetDC(), 0, 0, SRCCOPY);
		m_imageOriginal.ReleaseDC();
	}

	if (FAILED(hr)) {
		AfxMessageBox(_T("Failed to load image format."));
		return FALSE;
	}

	return TRUE;
}

BOOL Cproject1Doc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// 1. Double check that we actually have an image buffer loaded in RAM
	if (m_image.IsNull())
	{
		AfxMessageBox(_T("No active image found to save."));
		return FALSE;
	}

	// 2. Save the CImage memory layer directly back to disk.
	// CImage automatically detects the format (BMP, JPG, PNG) using the file extension.
	HRESULT hr = m_image.Save(lpszPathName);

	if (FAILED(hr))
	{
		AfxMessageBox(_T("Failed to save the image file. Make sure the file isn't write-protected."));
		return FALSE;
	}

	// 3. Clear the dirty modification flag since the disk matches our RAM workspace now
	SetModifiedFlag(FALSE);

	return TRUE;
}

BOOL Cproject1Doc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	CString strPathName = lpszPathName;

	// If no path was given, this is a "Save As" -> ask the user
	if (lpszPathName == NULL)
	{
		CString filter;
		filter = _T("PNG Image (*.png)|*.png|")
			_T("JPEG Image (*.jpg)|*.jpg|")
			_T("Bitmap Image (*.bmp)|*.bmp|")
			_T("TIFF Image (*.tif)|*.tif||");

		CFileDialog dlg(FALSE,
			_T("png"),
			GetPathName().IsEmpty() ? _T("Untitled") : GetPathName(),
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,
			filter);

		if (dlg.DoModal() != IDOK)
			return FALSE;

		strPathName = dlg.GetPathName();
	}

	// ---------- STEP 6 GOES HERE ----------
	// Validate the extension before attempting to save
	CString ext = strPathName.Mid(strPathName.ReverseFind(_T('.')) + 1).MakeLower();
	if (ext != _T("png") && ext != _T("jpg") && ext != _T("jpeg") &&
		ext != _T("bmp") && ext != _T("tif") && ext != _T("tiff"))
	{
		AfxMessageBox(_T("Unsupported file format. Please use PNG, JPG, BMP, or TIFF."));
		return FALSE;
	}
	// ---------------------------------------

	// Reuse your existing save logic
	if (!OnSaveDocument(strPathName))
		return FALSE;

	if (bReplace)
	{
		SetPathName(strPathName);
	}

	return TRUE;
}

void Cproject1Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void Cproject1Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void Cproject1Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void Cproject1Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Cproject1Doc diagnostics

#ifdef _DEBUG
void Cproject1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void Cproject1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Cproject1Doc commands



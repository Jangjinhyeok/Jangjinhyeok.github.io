(function () {
  // Render every <script type="text/x-mermaid"> block into an inline SVG.
  mermaid.initialize({
    startOnLoad: false,
    securityLevel: 'strict',
    theme: 'base',
    fontFamily: 'Pretendard, "Malgun Gothic", sans-serif',
    themeVariables: {
      fontSize: '14px',
      primaryColor: '#eef2fc',
      primaryBorderColor: '#8aa3e6',
      primaryTextColor: '#1d1f23',
      lineColor: '#7a808a',
      secondaryColor: '#f6f2ec',
      tertiaryColor: '#f7f8fa',
      clusterBkg: '#f7f8fa',
      clusterBorder: '#d7dbe2',
      noteBkgColor: '#fff8e6',
      noteBorderColor: '#e6cf8f'
    },
    flowchart: { htmlLabels: true, curve: 'basis' },
    sequence: { mirrorActors: false },
    gantt: { barHeight: 18, fontSize: 12 }
  });

  const sources = Array.from(document.querySelectorAll('script[type="text/x-mermaid"]'));
  sources.forEach(async (src, i) => {
    const box = document.createElement('div');
    box.className = 'diagram';
    src.replaceWith(box);
    const text = src.textContent.trim();
    try {
      const { svg } = await mermaid.render('mmd-' + i, text);
      box.innerHTML = svg;
    } catch (err) {
      box.classList.add('error');
      box.textContent = 'Diagram error: ' + (err && err.message ? err.message : err) + '\n\n' + text;
    }
  });

  // Highlight the current section in the table of contents.
  const links = Array.from(document.querySelectorAll('.toc a'));
  const byId = new Map(links.map(a => [a.getAttribute('href').slice(1), a]));
  const observer = new IntersectionObserver(entries => {
    entries.forEach(e => {
      if (!e.isIntersecting) return;
      links.forEach(a => a.classList.remove('active'));
      const a = byId.get(e.target.id);
      if (a) a.classList.add('active');
    });
  }, { rootMargin: '-20% 0px -70% 0px' });
  byId.forEach((_, id) => { const el = document.getElementById(id); if (el) observer.observe(el); });

  // Pause looping videos that are off screen; add a still poster for print.
  const vids = Array.from(document.querySelectorAll('video'));
  vids.forEach(v => { const img = document.createElement('img'); img.className = 'print-poster'; img.alt = ''; img.src = v.getAttribute('poster'); v.after(img); });
  const vObs = new IntersectionObserver(entries => {
    entries.forEach(e => { if (e.isIntersecting) e.target.play().catch(() => {}); else e.target.pause(); });
  });
  vids.forEach(v => vObs.observe(v));
})();

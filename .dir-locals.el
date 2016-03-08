(setq company-c-headers-path-user
      '("common/include"
       "server/include"
       "client/include"
       )
      )

(setq blocks-root (substring (pwd) 10))
;; (setq block-includes
;;       (mapcar
;;        (lambda (path) (concat "-I" blocks-root path))
;;        '("/common/include/"
;;          "/common/generated_include/"
;;          "/server/include/"
;;          "/client/include/"
;;          "third-party/entityx/entityx/"
;;          "third-party/duktape"
;;          "../panda3d/built/include/"
;;          )))
;; (setq company-clang-arguments
;;       (append
;;        company-clang-arguments
;;        '("-std=c++14")
;;        block-includes
;;        ))
